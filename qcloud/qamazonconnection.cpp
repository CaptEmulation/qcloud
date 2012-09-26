#include "qamazonconnection.h"
#include <QDebug>
#include <QSslSocket>

#include <QDateTime>
#include <QEventLoop>

QAmazonConnection::QAmazonConnection(QByteArray user, QByteArray password, QByteArray secret) {
    this->host = "s3.amazonaws.com";
    this->username = user;
    this->password = password;
    this->secret = secret;
    manager = new QNetworkAccessManager(this);
}

QAmazonConnection::~QAmazonConnection() {
    manager->deleteLater();
}

bool QAmazonConnection::deleteBlob(QString name, QString bucket) {
    Request r;
    r.headers.insert("verb", "DELETE");
    r.headers.insert("bucket", bucket);
    r.headers.insert("filename", name);
    return true;
}

bool QAmazonConnection::deleteCloudDir(QString bucket) {
    Request r;
    r.headers.insert("verb", "DELETE");
    r.headers.insert("bucket", bucket);
    return true;
}

bool QAmazonConnection::createCloudDir(const QString &dirName) {
    QString region = "EU";

    QString msg("<CreateBucketConfiguration xmlns=\"http://s3.amazonaws.com/doc/2006-03-01/\"><LocationConstraint>EU</LocationConstraint></CreateBucketConfiguration >");
    Request r;
    r.headers.insert("verb", "PUT");
    r.headers.insert("bucket", dirName);
    r.headers.insert("filesize", "0");
    QNetworkReply *reply;
    try {
        QByteArray data("");
        reply = sendPut(encode(r), data);
        r.headers.clear();
        reply->deleteLater();
        qDebug() << reply->readAll();
        return true;
    } catch (QString msg) {
        qDebug() << msg;
        reply->deleteLater();
        return false;
    }
}

bool QAmazonConnection::cloudDirExists(const QString &dirName) {
    Request r;
    r.headers.insert("verb", "HEAD");
    r.headers.insert("bucket", dirName);
    QNetworkReply *reply = sendHead(encode(r));
    if (reply->error() != 0) {
        qDebug() << QString("CloudDir %1 does not exist").arg(dirName);
        return false;
    }
    return true;
}

bool QAmazonConnection::put(QCloudDir &d) {
    disconnect(manager, 0, this, 0);
    QString path = d.getPath();

    if (!cloudDirExists(path)) {
        createCloudDir(path);
    }

    QList<QString> contents = d.getCloudDirContentsAsString();
    int size = contents.size();

    emit setRange(0, size);

    if (!overrideCloud) {
        QList<QString> cloudcontents = getCloudDirContents(d.getPath());
        for (int i = 0 ; i < size; i++) {
            if (!cloudcontents.contains(contents.at(i))) {
                put((*d.get(i)), path);
                emit valueChanged(i);
            }
        }
    }
    else
    {
        for (int i = 0 ; i < size; i++) {
            put((*d.get(i)), path);
            emit valueChanged(i);
        }
    }

    emit putCloudDirFinished();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
    return true;
}

bool QAmazonConnection::get(QCloudDir &d) {

    QString path = d.getPath();
    QDir local(path);
    if (local.exists()) {
        local.rename(local.dirName(), local.dirName()+"new");
    }
    qDebug() << QString("Getting QCD %1").arg(path);
    disconnect(manager, 0, this, 0);

    QList<QString> files = getCloudDirContents(path);
    int size = files.length();
    emit setRange(0, files.length());

    if (!overrideLocal) {
        QList<QString> localFiles = d.getCloudDirContentsAsString();
        for (int i = 0; i < size; i++) {
            if (!files.contains(localFiles.at(i))) {
                d.add(get(path, files.at(i)));
                emit valueChanged(i);
            }
        }
    }
    else
    {
        for (int i = 0; i < files.length(); i++) {
            d.add(get(path, files.at(i)));
            emit valueChanged(i);
        }
    }

    emit getCloudDirFinished();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
    return true;
}



QCloudFile* QAmazonConnection::get(QString bucket, QString fileName) {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET");
    r.headers.insert("bucket", bucket);
    r.headers.insert("filename", fileName);

    try {
        reply = sendGet(encode(r));
        r.headers.clear();
        QCloudFile* file = new QCloudFile(reply->readAll(), fileName, bucket);
        reply->deleteLater();
        emit finished();
        return file;
    }
    catch (const char* msg)
    {
        qDebug() << msg;
        emit finished();
    }
}


QList<QString> QAmazonConnection::getCloudDir() {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET");
    reply = sendGet(encode(r));

    if (reply->error() == 0) {
        QByteArray data = reply->readAll();
        return parseCloudDirListings(data);
    }
    else
    {
        return QList<QString>();
    }

}


QList<QString> QAmazonConnection::getCloudDirContents(QString bucketName) {
    Request r;
    QNetworkReply *reply;

    r.headers.insert("verb", "GET");
    r.headers.insert("bucket", bucketName);

    reply = sendGet(encode(r));

    QByteArray array = reply->readAll();
    reply->deleteLater();
    return parseCloudDirContentListing(&array);
}

bool QAmazonConnection::put(QCloudTable &table) {
    return true;
}

void QAmazonConnection::replaceUnallowed(QByteArray *array) {
    array->replace('/', "%2F");
    array->replace('+', "%2B");
}


bool QAmazonConnection::put(QCloudFile &f, QString bucket) {
    Request r;
    r.headers.insert("verb","PUT");
    r.headers.insert("filename", f.getName());
    r.headers.insert("bucket", bucket);
    r.headers.insert("Content-Type", "text/plain");
    r.headers.insert("filesize", QByteArray::number(f.getSize()));

    QNetworkReply *reply;
    try {
        reply = sendPut(encode(r), f.getContents());
        r.headers.clear();
        reply->deleteLater();
        return true;
    } catch (QString msg){
        return false;
        reply->deleteLater();
    }
}

/**
  Creates the request that is then sent to Amazon, everything should be as general as possible and
  this should take care of getting buckets and files.
  */
QNetworkRequest QAmazonConnection::encode(const Request &r) {

    QString timeString = QString::number(QDateTime::currentMSecsSinceEpoch()/1000+200);
    QByteArray stringToSign = r.headers.value("verb").toAscii() + "\n";

    stringToSign += "\n\n";
    stringToSign += timeString + "\n";

    QString urlString = "";

    if (r.headers.contains("bucket")) {
        QString bucket = r.headers.value("bucket");
        stringToSign += "/" + bucket + "/";
        urlString = "http://" + bucket + "." + this->host + "/";

        if (r.headers.contains("filename")) {
            QString value = r.headers.value("filename");
            urlString += value;
            stringToSign += value;
        }
    } else {
        stringToSign += "/";
        urlString = "http://" + this->host + "/";
    }

    QUrl url(urlString);

    QByteArray hashedSignature = HmacSHA::hash(HmacSHA::HmacSHA1, stringToSign, this->secret);


    replaceUnallowed(&hashedSignature);
    QNetworkRequest req;

    url.addEncodedQueryItem("AWSAccessKeyId", this->password);
    url.addEncodedQueryItem("Signature", hashedSignature);
    if(r.headers.value("verb") == "PUT") {
        QString value = r.headers.value("filesize");
        if (value != "0") url.addEncodedQueryItem("Content-Type", r.headers.value("Content-Type").toAscii());
        url.addEncodedQueryItem("Content-Length", value.toAscii());
    }
    url.addEncodedQueryItem("Expires", timeString.toAscii());

    req.setUrl(url);
    return req;
}

void QAmazonConnection::setOverrideCloud(bool value) {
    this->overrideCloud = value;
}

void QAmazonConnection::setOverrideLocal(bool value) {
    this->overrideLocal = value;
}

QNetworkReply* QAmazonConnection::sendPut(const QNetworkRequest &r, const QByteArray &payload) {
    QEventLoop l;
    QNetworkReply *reply = manager->put(r, payload);
    connect(reply, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();

    return reply;
}

QNetworkReply* QAmazonConnection::sendGet(const QNetworkRequest &r) {
    QEventLoop l;
    QNetworkReply *reply = manager->get(r);
    connect(reply, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();

    return reply;
}

QNetworkReply* QAmazonConnection::sendHead(const QNetworkRequest &r) {
    QEventLoop l;
    QNetworkReply *reply = manager->head(r);
    connect(reply, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();
    return reply;
}

/**
  Will be changed to use QVector<QCloud> tahi QVector<QCloudBucket *>, and there will be a
  matching parseBucketContentsListing().
  */
QList<QString> QAmazonConnection::parseCloudDirListings(QByteArray &message){
    QXmlStreamReader reader;
    reader.addData(message);
    QList<QString> list;
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.name().toString() == "Name"){
            list.append(reader.readElementText());
        }
    }
    return list;
}

QList<QString> QAmazonConnection::parseCloudDirContentListing(QByteArray *message) {
    QXmlStreamReader reader;
    reader.addData(*message);
    QList<QString> files;
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name().toString() == "Key") {
            files.append(reader.readElementText());
        }
    }
    emit finished();
    return files;
}

QCloudResponse::RESPONSETYPE QAmazonConnection::findType(QNetworkReply &reply, QByteArray &contents) {
    contents = reply.readAll();
    qDebug() << reply.errorString();
    QXmlStreamReader reader;
    reader.addData(contents);
    reader.readNextStartElement();
    qDebug() << reader.name().toString();
    if (reader.name().toString() == "ListAllMyBucketsResult") {
        return QCloudResponse::CLOUDDIR;
    } else if (reader.name().toString() == "ListBucketResult") {
               return QCloudResponse::CLOUDDIRCONTENTS;
    } else return QCloudResponse::CLOUDFILE;
}

//SLOTS
void QAmazonConnection::requestFinished(QNetworkReply *reply) {
    int error = reply->error();
    QCloudResponse::RESPONSETYPE type;
    QByteArray cont = "";
    if (error == 0) {
        type = static_cast<QCloudResponse::RESPONSETYPE>(findType(*reply, cont));
    } else {
        cont = reply->readAll();
        type = QCloudResponse::CLOUDDIR;
    }
    QCloudResponse *resp = new QCloudResponse(cont, error, type);
    reply->deleteLater();
    emit cloudRequestFinished(resp);
}

