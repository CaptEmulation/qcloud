#include "qazureconnection.h"
#include <QDateTime>

QAzureConnection::~QAzureConnection() {
    manager->deleteLater();
}

QAzureConnection::QAzureConnection(QByteArray url, QByteArray authentication, QByteArray storageKey) {
    this->url = url;
    this->authentication = authentication;
    this->storageKey = storageKey;
    manager = new QNetworkAccessManager(this);
    if (authentication == "SharedKey") initializeHeaders();
    else initializeSharedKeyLiteHeaders();
}

void QAzureConnection::initializeHeaders() {
    this->head.requiredHeaders.append(qMakePair(QString("Content-Encoding"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Content-Language"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Content-MD5"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Content-Type"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Date"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("If-Modified-Since"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("If-Match"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("If-None-Match"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("If-Unmodified-Since"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Range"), QString("\n")));
}

void QAzureConnection::initializeSharedKeyLiteHeaders() {
    this->head.requiredHeaders.append(qMakePair(QString("Content-MD5"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Content-Type"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Date"), QString("\n")));
}

QString QAzureConnection::dateInRFC1123() {
    QString httpDate;
    QDateTime dDate = QDateTime::currentDateTimeUtc();
    QLocale locale( QLocale::C );
    QDateTime utcDate = dDate.toTimeSpec( Qt::UTC );
    httpDate += locale.toString( utcDate.date(), "ddd, dd MMM yyyy" );
    httpDate += utcDate.toString( " hh:mm:ss" );
    httpDate += " GMT";
    return httpDate;
}


bool QAzureConnection::put(QByteArray &array, QString fileName, QString bucket) {
    Request r;
    QNetworkReply *reply;

    r.headers.insert("verb", "PUT\n");
    r.headers.insert("path", "/" + bucket + "/" + fileName);
    r.headers.insert("size", QByteArray::number(array.size()));

    reply = sendData(encode(r), array);
    if (reply->error() > 0) {
        return false;
    }
    return true;
}

bool QAzureConnection::put(QCloudFile &f, QString bucket) {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "PUT\n");
    r.headers.insert("path", "/" + bucket + "/" + f.getName());
    r.headers.insert("size", QByteArray::number(f.getSize()));

    reply = sendData(encode(r), f.getContents());
    r.headers.clear();
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
        return false;
    }
    return true;
}

bool QAzureConnection::put(QCloudTable &table){
    return true;
}



QList<QString> QAzureConnection::getBuckets() {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET\n");
    r.headers.insert("path", "/");
    r.headers.insert("operation", "comp=list");
    reply = sendData(encode(r));
    r.headers.clear();
    QByteArray contents = reply->readAll();
    return parseBucketListing(contents);
}

QList<QString> QAzureConnection::getBucketContents(QString bucketName) {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET\n");
    r.headers.insert("path", "/" + bucketName);
    r.headers.insert("operation", "comp=list&restype=container");

    reply = sendData(encode(r));
    r.headers.clear();
    QByteArray contents = reply->readAll();
    reply->deleteLater();
    return parseBucketContentsListing(contents);
}


QByteArray* QAzureConnection::get(QString name) {
    return new QByteArray();
}

QByteArray* QAzureConnection::get(QString bucket, QString name){
    Request r;
    QNetworkReply *reply;

    r.headers.insert("verb", "GET\n");
    r.headers.insert("path", "/" + bucket + "/" + name);

    reply = sendData(encode(r));
    QByteArray *info = new QByteArray(reply->readAll());
    reply->deleteLater();
    return info;
}


bool QAzureConnection::createContainer(QString name) {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "PUT\n");
    r.headers.insert("path", "/" + name);
    r.headers.insert("operation", "restype=container");

    reply = sendData(encode(r), QByteArray(""));
    r.headers.clear();


    if (reply->error() > 0) {
        reply->deleteLater();
        return false;
    }
    reply->deleteLater();
    return true;
}

QNetworkRequest QAzureConnection::encode(const Request &r) {
    QString urlString;

    if (r.headers.value("verb") == "PUT\n") urlString = this->url + r.headers.value("path");
    else if (r.headers.contains("path")) urlString = this->url + r.headers.value("path") + "?" +r.headers.value("operation");
    else if (r.headers.value("verb") == "GET\n" && !r.headers.contains("operation")) urlString = this->url + r.headers.value("path");
    else urlString = this->url + "?" + r.headers.value("operation");

    QUrl url = QUrl::fromEncoded(urlString.toAscii());
    QNetworkRequest req;
    req.setUrl(url);

    QByteArray stringToSign;
    stringToSign.append(r.headers.value("verb"));

    for (int i = 0; i < head.requiredHeaders.size(); i++) {
        if (head.requiredHeaders.at(i).first == "Content-MD5" && r.headers.value("verb") == "PUT\n" && !r.headers.contains("size")){
            stringToSign += "0\n";
        } else if (r.headers.contains("size") && head.requiredHeaders.at(i).first == "Content-MD5") {
            stringToSign += r.headers.value("size") + "\n";
        }
        else stringToSign += head.requiredHeaders.at(i).second;
    }

    QString date = dateInRFC1123();

    if (r.headers.contains("size")) {
       // stringToSign += "Content-Length:" + r.headers.value("size");
        stringToSign += "\nx-ms-blob-type:BlockBlob";
    }
    stringToSign += "\nx-ms-date:" + date;
    stringToSign += "\nx-ms-version:2009-09-19\n";

    stringToSign += "/jagebage" + r.headers.value("path");
    if (r.headers.contains("operation")){
        QString temp = r.headers.value("operation");
        stringToSign += "\n" + temp.replace("=", ":").replace("&", "\n");
    }

    QByteArray hash = HmacSHA::hash(HmacSHA::HmacSHA256, stringToSign, this->storageKey);

    QString version = "2009-09-19";
    QByteArray test = this->authentication + " jagebage:" + hash;

    req.setRawHeader("Authorization", test);
    req.setRawHeader("x-ms-date", date.toAscii());
    req.setRawHeader("x-ms-version", version.toAscii());
    if(r.headers.value("verb") == "PUT\n")  {
        req.setRawHeader("Content-Length", r.headers.value("size").toAscii());
        req.setRawHeader("x-ms-blob-type", "BlockBlob");
    }
    return req;
}

//SEND FUNCTIONS
QNetworkReply* QAzureConnection::sendData(const QNetworkRequest &req) {
    QEventLoop loop;
    QNetworkReply *reply;
    reply = manager->get(req);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
    }
    return reply;
}

QNetworkReply* QAzureConnection::sendData(const QNetworkRequest &req, const QByteArray &payload) {
   QEventLoop loop;
   QNetworkReply *reply;
   reply = manager->put(req, payload);
   connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
   loop.exec();

   if (reply->error() > 0) {
       qDebug() << reply->errorString();
   }
   return reply;
}

//PARSERS
QList<QString> QAzureConnection::parseBucketListing(QByteArray &contents) {
    QXmlStreamReader *reader = new QXmlStreamReader();
    reader->addData(contents);
    QList<QString> foo;
    while (!reader->atEnd()) {
        reader->readNextStartElement();
        if (reader->name().toString() == "Name") {
            foo.append(reader->readElementText());
        }
    }
    reader->~QXmlStreamReader();
    return foo;
}

QList<QString> QAzureConnection::parseBucketContentsListing(QByteArray &contents) {
    QXmlStreamReader *reader = new QXmlStreamReader();
    QList<QString> foo;
    reader->addData(contents);
    while (!reader->atEnd()) {
        reader->readNextStartElement();
        if (reader->name().toAscii() == "Name") foo.append(reader->readElementText());
    }
    reader->~QXmlStreamReader();
    return foo;
}
