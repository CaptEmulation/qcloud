#include "qamazonconnection.h"
#include <QDebug>
#include <QSslSocket>

#include <QDateTime>
#include <QEventLoop>

QAmazonConnection::QAmazonConnection() {}

QAmazonConnection::QAmazonConnection(QByteArray host, QByteArray user, QByteArray password, QByteArray secret) {
    this->c.info.insert("host", host);
    this->c.info.insert("username", user);
    this->c.info.insert("password", password);
    this->c.info.insert("secret", secret);
    manager = new QNetworkAccessManager(this);
    reader = new QXmlStreamReader();
}

bool QAmazonConnection::put(QCloudFile &f, QString bucket) {
    Request r;
    r.headers.insert("verb","PUT");
    r.headers.insert("path", "/" + bucket + "/");
    r.headers.insert("fileName", f.getName());
    r.headers.insert("Content-Type", "text/plain");
    qDebug() << r.headers.value("fileSize");

    try {
        reply = sendData(encode(r), f.getContents());
        headers.clear();
    } catch (const char* msg){
        headers.clear();
        return false;
    }
    return true;
}

bool QAmazonConnection::put(QCloudTable &table) {
    return true;
}

void QAmazonConnection::replaceUnallowed(QByteArray *array) {
    array->replace('/', "%2F");
    array->replace('+', "%2B");
}

QByteArray* QAmazonConnection::get(QString bucket, QString fileName) {
    Request r;
    r.headers.insert("verb", "GET");
    r.headers.insert("path","/" + bucket + "/");
    r.headers.insert("fileName", fileName);
    try {
        reply = sendData(encode(r));
    } catch (const char* msg) {
        qDebug() << msg;
    }

    headers.clear();
    QByteArray array = reply->readAll();
    reply->deleteLater();
    return &array;
}

QNetworkReply* QAmazonConnection::sendData(const QNetworkRequest req, const QByteArray &payload) {
    reply = manager->put(req, payload);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    if (reply->error() > 0){
        qDebug() << reply->errorString();
        throw "Error sending data, with error " + reply->errorString();
    }
    loop.deleteLater();
    return reply;
}

QNetworkReply* QAmazonConnection::sendData(QNetworkRequest req) {
    qDebug() << "@ senddata" ;
    qDebug() << req.rawHeaderList() << " " << req.url();
    reply = manager->get(req);
    QEventLoop loop;

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
        throw "Error sending data. Error message: " + reply->errorString();
    }
    loop.deleteLater();

    return reply;
}

QList<QString> QAmazonConnection::getBuckets() {
    Request r;
    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/");

    reply = sendData(encode(r));

    QByteArray msg = reply->readAll();
    qDebug() << msg;
    QList<QString> list = parseBucketListings(&msg);

    reply->deleteLater();
    return list;
}


QList<QString> QAmazonConnection::getBucketContents(QString bucketName) {
    qDebug() << "from getBucketContents";
    Request r;
    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/" + bucketName + "/");
    reply = sendData(encode(r));
    QByteArray array = reply->readAll();
    reply->deleteLater();
    return parseBucketContentListing(&array);
}


/**
  Creates the request that is then sent to Amazon, everything should be as general as possible and
  this should take care of getting buckets and files.
  */
QNetworkRequest QAmazonConnection::encode(const Request r) {
    QString timeString = QString::number(QDateTime::currentMSecsSinceEpoch()/1000+200);
    QByteArray stringToSign;
    QString path = r.headers.value("path");

    stringToSign += r.headers.value("verb") + "\n";
    if (r.headers.contains("Content-Type")){
        stringToSign += "\n";
        stringToSign += r.headers.value("Content-Type") + "\n";
    } else {
         stringToSign += "\n\n";
    }

    stringToSign += timeString + "\n";
    stringToSign += path;
    if (r.headers.contains("fileName")) {
        stringToSign += r.headers.value("fileName");
    }
    QUrl url;
    qDebug() << stringToSign;
    if (r.headers.contains("fileName")) {
        url = QUrl("http://" + path.replace("/", "") + "." + this->c.info.value("host")+"/" + r.headers.value("fileName"));
    } else if ( r.headers.value("path") != "/"){
        url = QUrl("http://" + path.replace("/", "") + "." + this->c.info.value("host")+"/");
    } else {
        url = QUrl("http://" + this->c.info.value("host") + "/");
    }

    QByteArray hashedSignature = HmacSHA::hash(HmacSHA::HmacSHA1, stringToSign, this->c.info.value("secret"));

    replaceUnallowed(&hashedSignature);

    QNetworkRequest req;

    url.addEncodedQueryItem("AWSAccessKeyId", this->c.info.value("password"));
    url.addEncodedQueryItem("Signature", hashedSignature);
    if(r.headers.value("verb") == "PUT") {
        url.addEncodedQueryItem("Content-Type", r.headers.value("Content-Type").toAscii());
        url.addEncodedQueryItem("Content-Length", r.headers.value("fileSize").toAscii());
    }
    url.addEncodedQueryItem("Expires", timeString.toAscii());
    req.setUrl(url);
    return req;
}


/**
  Will be changed to use QVector<QCloudBucket> tahi QVector<QCloudBucket *>, and there will be a
  matching parseBucketContentsListing().
  */
QList<QString> QAmazonConnection::parseBucketListings(QByteArray *message){
    reader->addData(*message);
    QList<QString> list;
    while (!reader->atEnd()) {
        reader->readNextStartElement();
        if (reader->name().toString() == "Name"){
            list.append(reader->readElementText());
        }
    }
    reader->clear();

    return list;
}

QList<QString> QAmazonConnection::parseBucketContentListing(QByteArray *message) {
    reader->addData(*message);
    QList<QString> files;
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->name().toString() == "Key") {
            files.append(reader->readElementText());
        }
    }
    reader->clear();
    return files;
}
