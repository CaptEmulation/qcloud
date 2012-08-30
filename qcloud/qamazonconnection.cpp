#include "qamazonconnection.h"
#include <QDebug>
#include <QSslSocket>

#include <QDateTime>
#include <QEventLoop>

QAmazonConnection::QAmazonConnection(QByteArray host, QByteArray user, QByteArray password, QByteArray secret) {
    this->host = host;
    this->username = user;
    this->password = password;
    this->secret = secret;
    manager = new QNetworkAccessManager(this);
}

QAmazonConnection::~QAmazonConnection() {
    manager->deleteLater();
}

bool QAmazonConnection::deleteBlob(QString name, QString bucket) {
    return true;
}

bool QAmazonConnection::deleteBucket(QString bucket) {
    return true;
}

bool QAmazonConnection::put(QByteArray &array, QString fileName, QString bucket) {
    Request r;
    r.headers.insert("verb", "PUT");
    r.headers.insert("path", "/"+ bucket + "/");
    r.headers.insert("fileName", fileName);
    r.headers.insert("Content-Type", "text/plain");
    r.headers.insert("fileSize", QByteArray::number(array.size()));

    QNetworkReply *reply;
    try {
        reply = sendData(encode(r), array);
        r.headers.clear();
        reply->deleteLater();
        return true;
    } catch (QString msg) {
        qDebug() << msg;
        reply->deleteLater();
        return false;
    }
}

bool QAmazonConnection::put(QCloudFile &f, QString bucket) {
    Request r;
    r.headers.insert("verb","PUT");
    r.headers.insert("path", "/" + bucket + "/");
    r.headers.insert("fileName", f.getName());
    r.headers.insert("Content-Type", "text/plain");
    r.headers.insert("fileSize", QByteArray::number(f.getSize()));

    QNetworkReply *reply;
    try {
        reply = sendData(encode(r), f.getContents());
        r.headers.clear();
        reply->deleteLater();
        return true;
    } catch (QString msg){
        return false;
        reply->deleteLater();
    }
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
    QNetworkReply *reply;
    r.headers.insert("verb", "GET");
    r.headers.insert("path","/" + bucket + "/");
    r.headers.insert("fileName", fileName);

    try {
        reply = sendData(encode(r));
        r.headers.clear();
        QByteArray *array = new QByteArray(reply->readAll());
        reply->deleteLater();
        return array;
    } catch (const char* msg) {
        qDebug() << msg;
        return new QByteArray("");
    }
}



QList<QString> QAmazonConnection::getBuckets() {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/");

    reply = sendData(encode(r));

    QByteArray msg = reply->readAll();

    QList<QString> list = parseBucketListings(&msg);

    reply->deleteLater();
    return list;
}


QList<QString> QAmazonConnection::getBucketContents(QString bucketName) {
    Request r;
    QNetworkReply *reply;

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
QNetworkRequest QAmazonConnection::encode(const Request &r) {
    QString timeString = QString::number(QDateTime::currentMSecsSinceEpoch()/1000+200);

    QString path = r.headers.value("path");
    QByteArray stringToSign = r.headers.value("verb").toAscii() + "\n";

    stringToSign += "\n\n";
    stringToSign += timeString + "\n";
    stringToSign += path;
    if (r.headers.contains("fileName")) {
        stringToSign += r.headers.value("fileName");
    }
    QUrl url;

    if (r.headers.contains("fileName")) {
        url = QUrl("http://" + path.replace("/", "") + "." + this->host +"/" + r.headers.value("fileName"));
    } else if ( r.headers.value("path") != "/"){
        url = QUrl("http://" + path.replace("/", "") + "." + this->host +"/");
    } else {
        url = QUrl("http://" + this->host + "/");
    }

    QByteArray hashedSignature = HmacSHA::hash(HmacSHA::HmacSHA1, stringToSign, this->secret);


    replaceUnallowed(&hashedSignature);
    QNetworkRequest req;

    url.addEncodedQueryItem("AWSAccessKeyId", this->password);
    url.addEncodedQueryItem("Signature", hashedSignature);
    if(r.headers.value("verb") == "PUT") {
        url.addEncodedQueryItem("Content-Type", r.headers.value("Content-Type").toAscii());
        url.addEncodedQueryItem("Content-Length", r.headers.value("fileSize").toAscii());
    }
    url.addEncodedQueryItem("Expires", timeString.toAscii());
    req.setUrl(url);

    return req;
}

QNetworkReply* QAmazonConnection::sendData(const QNetworkRequest &req, const QByteArray &payload) {
    QNetworkReply *reply;
    reply = manager->put(req, payload);
    QEventLoop loop;

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (reply->error() > 0){
        qDebug() << reply->readAll();
        throw "Error sending data, with error " + reply->errorString();
    }
    loop.deleteLater();
    return reply;
}

QNetworkReply* QAmazonConnection::sendData(const QNetworkRequest &req) {
    QNetworkReply *reply;
    reply = manager->get(req);
    QEventLoop loop;

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (reply->error() > 0) {
        throw "Error sending data. Error message: " + reply->errorString();
    }
    loop.deleteLater();

    return reply;
}

/**
  Will be changed to use QVector<QCloudBucket> tahi QVector<QCloudBucket *>, and there will be a
  matching parseBucketContentsListing().
  */
QList<QString> QAmazonConnection::parseBucketListings(QByteArray *message){
    QXmlStreamReader reader;
    reader.addData(*message);
    QList<QString> list;
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.name().toString() == "Name"){
            list.append(reader.readElementText());
        }
    }
    return list;
}

QList<QString> QAmazonConnection::parseBucketContentListing(QByteArray *message) {
    QXmlStreamReader reader;
    reader.addData(*message);
    QList<QString> files;
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name().toString() == "Key") {
            files.append(reader.readElementText());
        }
    }
    return files;
}
