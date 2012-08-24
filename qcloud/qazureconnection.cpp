#include "qazureconnection.h"
#include <QDateTime>

QAzureConnection::QAzureConnection()
{
    manager = new QNetworkAccessManager();
}

QAzureConnection::QAzureConnection(QByteArray url, QString authentication ,QByteArray storageKey) {
    this->c.info.insert("url", url);
    this->c.info.insert("storageKey", storageKey);
    this->c.info.insert("authentication", authentication.toAscii());
    manager = new QNetworkAccessManager(this);
    reader = new QXmlStreamReader();
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



bool QAzureConnection::put(QCloudFile &f, QString bucket) {
    Request r;
    r.headers.insert("verb", "PUT\n");
    r.headers.insert("path", "/" + bucket + "/" + f.getName());
    r.headers.insert("size", QByteArray::number(f.getSize()));

    reply = sendData(encode(r), f.getContents());
    r.headers.clear();
    if (reply->error() > 0) {
        return false;
    }
    return true;
}

bool QAzureConnection::put(QCloudTable &table){
    return true;
}



QList<QString> QAzureConnection::getBuckets() {
    Request r;
    r.headers.insert("verb", "GET\n");
    r.headers.insert("path", "/");
    r.headers.insert("operation", "comp=list");
    reply = sendData(encode(r));
    r.headers.clear();
    return parseBucketListing(reply);
}

QList<QString> QAzureConnection::getBucketContents(QString bucketName) {
    Request r;
    r.headers.insert("verb", "GET\n");
    r.headers.insert("path", "/" + bucketName);
    r.headers.insert("operation", "comp=list&restype=container");

    reply = sendData(encode(r));
    r.headers.clear();
    return parseBucketContentsListing(reply);
}


QByteArray* QAzureConnection::get(QString name) {
    return new QByteArray();
}

QByteArray* QAzureConnection::get(QString bucket, QString name){
    return 0;
}


bool QAzureConnection::createContainer(QString name) {
    Request r;
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

QNetworkRequest QAzureConnection::encode(const Request r){
    QString urlString;

    if (r.headers.contains("path")) urlString = c.info.value("url") + r.headers.value("path") + "?" +r.headers.value("operation");
    else urlString = c.info.value("url") + "?" + r.headers.value("operation");

    QUrl url = QUrl::fromEncoded(urlString.toAscii());
    QNetworkRequest req;
    req.setUrl(url);

    QByteArray stringToSign;
    stringToSign.append(r.headers.value("verb"));

    for (int i = 0; i < head.requiredHeaders.size(); i++) {
        if (head.requiredHeaders.at(i).first == "Content-MD5" && r.headers.value("verb") == "PUT\n"){
            stringToSign += "0\n";
        } else if (r.headers.contains("size") && head.requiredHeaders.at(i).first == "Content-Size") {
            stringToSign += r.headers.value("size") + "\n";
        }
        else stringToSign += head.requiredHeaders.at(i).second;
    }

    QString date = dateInRFC1123();

    stringToSign += "\nx-ms-date:" + date;
    stringToSign += "\nx-ms-version:2009-09-19\n";
    stringToSign += "/jagebage" + r.headers.value("path");
    QString temp = r.headers.value("operation");
    stringToSign += "\n" + temp.replace("=", ":").replace("&", "\n");
    qDebug() << stringToSign;

    QByteArray hash = HmacSHA::hash(HmacSHA::HmacSHA256, stringToSign, this->c.info.value("storageKey"));

    QString version = "2009-09-19";
    QByteArray test = this->c.info.value("authentication") + " jagebage:" + hash;
    qDebug() << test;
    req.setRawHeader("Authorization", test);
    req.setRawHeader("x-ms-date", date.toAscii());
    req.setRawHeader("x-ms-version", version.toAscii());

    return req;
}

//SEND FUNCTIONS
QNetworkReply* QAzureConnection::sendData(const QNetworkRequest req) {
    QEventLoop loop;
    reply = manager->get(req);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
    }
    return reply;
}

QNetworkReply* QAzureConnection::sendData(const QNetworkRequest req, const QByteArray &payload) {
   QEventLoop loop;
   reply = manager->put(req, payload);
   connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
   loop.exec();

   if (reply->error() > 0) {
       qDebug() << reply->errorString();
   }
   qDebug() << reply->readAll();
   return reply;
}

//PARSERS
QList<QString> QAzureConnection::parseBucketListing(QNetworkReply *reply) {
    reader->addData(reply->readAll());
    reply->deleteLater();
    QList<QString> contents;
    while (!reader->atEnd()) {
        reader->readNextStartElement();
        if (reader->name().toString() == "Name") {
            contents.append(reader->readElementText());
        }
    }
    reader->clear();
    return contents;
}

QList<QString> QAzureConnection::parseBucketContentsListing(QNetworkReply *reply) {
    reader->addData(reply->readAll());
    reply->deleteLater();
    while (!reader->atEnd()) {
        reader->readNextStartElement();
        qDebug() << reader->readElementText();
    }
    return QList<QString>();
}
