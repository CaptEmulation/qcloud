#include "qazureconnection.h"
#include <QDateTime>

QAzureConnection::~QAzureConnection() {
    manager->deleteLater();
}

/*!
  \class QAzureConnection
  \brief Implementation of the interface QCloudConnection for Azure.

  Constructor to create new QAzureConnections. This contains three parameters and all should be in the right format.
  The first parameter is QByteArray containing the url of the service i.e. "kikkare.blob.core.windows.net" where
  kikkare is the storage accounts name. Authentication contains either SharedKey or SharedKeyLite. At the moment, this
  library has better support for SharedKey authentication. storageKey parameter should be made with QByteArray::fromBase64()
  as the key provided by Microsoft is Base64 encoded. The constructor also creates a new Headers-struct according to the
  provided authentication type.

  \sa QCloudConnection
  */
QAzureConnection::QAzureConnection(QByteArray url, QByteArray storageAccountName, QByteArray storageKey) {
    this->url = url;
    this->storageAccountName = storageAccountName;
    this->storageKey = storageKey;
    manager = new QNetworkAccessManager(this);
    initializeHeaders();
}

/*!
    \internal
    \fn QAzureConnection::initializeHeaders()
    \brief Initializes headers used in the signing of SharedKey requests */
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
    this->authentication = "SharedKey";
}

/*!
  \internal
  \brief Initializes headers to be used with SharedKeyLite requests.
*/
void QAzureConnection::initializeSharedKeyLiteHeaders() {
    this->head.requiredHeaders.append(qMakePair(QString("Content-MD5"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Content-Type"), QString("\n")));
    this->head.requiredHeaders.append(qMakePair(QString("Date"), QString("\n")));
    this->authentication = "SharedKeyLite";
}

/*!
  \internal
  \brief Azure requires dates to be in RFC1123 format that Qt's QDateTime does not provide,
  so this helper provides it.
  */
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

/*!
  Deletes blob indicated by \a name in clouddir \a bucket
  */
bool QAzureConnection::deleteBlob(QString name, QString bucket) {
    Request r;
    r.headers.insert("verb", "GET");
    r.headers.insert("path", bucket + "/" + name);
    return true;
}

/*!
  \reimp
  */
bool QAzureConnection::deleteCloudDir(QString bucket) {
    Request r;
    r.headers.insert("path", bucket);
    return true;
}

/*!
  \reimp
  */
bool QAzureConnection::cloudDirExists(const QString &dirName) {
    Request r;
    QNetworkReply *reply;

    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/" + dirName);
    r.headers.insert("operation", "comp=metadata&restype=container");

    reply = sendGet(encode(r));

    //If the cloudDir does not exist in cloud the response is 404 or 403.
    //Azure does n8ot provide a API-call for checking the existance of containers.

    if (reply->error() != 0) {
        emit cloudError();
        return false;
    }
    emit finished();
    return true;
}

/*!
  \reimp
  */
bool QAzureConnection::createCloudDir(const QString &name) {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "PUT");
    r.headers.insert("path", "/" + name);
    r.headers.insert("operation", "restype=container");
    r.headers.insert("size", "0");

    reply = sendPut(encode(r), QByteArray(""));
    r.headers.clear();

    if (reply->error() > 0) {
        reply->deleteLater();
        emit cloudError();
        return false;
    }
    reply->deleteLater();
    emit finished();
    return true;
}

/*!
  \reimp
  */
bool QAzureConnection::put(QCloudFile &f, QString bucket) {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "PUT");
    r.headers.insert("path", "/" + bucket + "/" + f.getName());
    r.headers.insert("size", QByteArray::number(f.getSize()));
    r.headers.insert("bucket", bucket);
    reply = sendPut(encode(r), f.getContents());
    if (reply->error() != 0) {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
        emit cloudError();
        return false;
    }
    emit finished();
    return true;
}


/*!
  \reimp
  */
bool QAzureConnection::put(QCloudDir &dir) {
    QList<QString> contents = dir.getCloudDirContentsAsString();
    int size = contents.size();
    QString path = dir.getPath();
    if (!cloudDirExists(path)) {
        createCloudDir(path);
        qDebug() << QString("Created container %1 to azure").arg(path);
    }
    emit setRange(0, size);
    if (!overrideCloud) {
        QList<QString> cloudContents = getCloudDirContents(path);
        for (int i = 0; i < size; i++) {
            if (!cloudContents.contains(contents.at(i))) {
                put((*dir.get(i)), path);
                emit valueChanged(i);
            }
        }
    }
    else
    {
        for (int i = 0; i < size; i++) {
            put((*dir.get(i)), path);
            emit valueChanged(i);
        }
    }

    emit putCloudDirFinished();
    return true;
}


/*!
  \reimp
  */
bool QAzureConnection::get(QCloudDir &d) {
    QString path = d.getPath();
    QList<QString> contents;
    int size;

    if (d.isLocal()) {
        contents = d.getCloudDirContentsAsString();
        size = contents.size();
        emit setRange(0, size);

        if(!overrideLocal) {
            QList<QString> cloudContents = getCloudDirContents(d.getPath());
            int cloudsize = cloudContents.size();
            for (int i = 0; i < cloudsize; i++) {
                if (!contents.contains(cloudContents.at(i))) {
                    get(path, cloudContents.at(i));
                    emit valueChanged(i);
                }
            }
        }
        else
        {
                for (int i = 0; i < size; i++){
                get(path, d.get(i)->getName());
                emit valueChanged(i);
            }
        }
    }
    else
    {
        contents = getCloudDirContents(path);
        size = contents.size();
        emit setRange(0, size);
        for (int i = 0; i < contents.length(); i++){
            get(path, contents.at(i));
            emit valueChanged(i);
        }
    }
    emit getCloudDirFinished();
    return true;
}


/*!
  \reimp
  */
QList<QString> QAzureConnection::getCloudDir() {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/");
    r.headers.insert("operation", "comp=list");
    reply = sendGet(encode(r));
    r.headers.clear();
    QByteArray contents = reply->readAll();
    return parseCloudDirListings(contents);
}

/*!
  \reimp
  */
QList<QString> QAzureConnection::getCloudDirContents(QString bucketName) {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/" + bucketName);
    r.headers.insert("operation", "comp=list&restype=container");

    reply = sendGet(encode(r));
    r.headers.clear();
    QByteArray contents = reply->readAll();
    reply->deleteLater();
    return parseCloudDirListings(contents);
}

/*!
  \reimp
  */
QCloudFile* QAzureConnection::get(QString bucket, QString name){
    Request r;
    QNetworkReply *reply;

    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/" + bucket + "/" + name);
    r.headers.insert("bucket", bucket);
    reply = sendGet(encode(r));
    if (reply->error() != 0) {
        emit cloudError();
        return 0;
    }
    QCloudFile *f = new QCloudFile(reply->readAll(), name,bucket);
    reply->deleteLater();
    return f;
}

/*!
  \reimp
  */
QCloudFileResponse* QAzureConnection::asyncGetCloudFile(QString &bucket, QString &fileName) {
    Request r;
    QNetworkReply *reply;

    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/" + bucket + "/" + fileName);
    r.headers.insert("bucket", bucket);

    QNetworkRequest requ = encode(r);
    reply = manager->get(requ);

    return new QCloudFileResponse(reply, fileName, bucket);
}

/*!
  \reimp
  */
QCloudListResponse* QAzureConnection::asyncGetCloudDirContents(QString &cloudDir) {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/" + cloudDir);
    r.headers.insert("operation", "comp=list&restype=container");

    QNetworkRequest requ = encode(r);
    reply = manager->get(requ);
    return new QCloudListResponse(reply);
}

/*!
  \reimp
  */
QCloudListResponse* QAzureConnection::asyncGetCloudDir() {
    Request r;
    QNetworkReply *reply;
    r.headers.insert("verb", "GET");
    r.headers.insert("path", "/");
    r.headers.insert("operation", "comp=list");

    QNetworkRequest requ = encode(r);
    reply = manager->get(requ);
    return new QCloudListResponse(reply);
}

/*!
  \internal
  */
QNetworkRequest QAzureConnection::encode(const Request &r) {
    QString urlString("http://" + this->url);

    if (r.headers.contains("path")) {
        urlString += r.headers.value("path");
    }

    if (r.headers.contains("operation")) {
        urlString += "?" + r.headers.value("operation");
    }

    QUrl url = QUrl::fromEncoded(urlString.toUtf8());
    QNetworkRequest req;
    req.setUrl(url);

    QByteArray stringToSign;
    stringToSign.append(r.headers.value("verb"));
    stringToSign.append("\n");

    for (int i = 0; i < head.requiredHeaders.size(); i++) {
        if (head.requiredHeaders.at(i).first == "Content-MD5" && r.headers.value("verb") == "PUT" && !r.headers.contains("size")){
            stringToSign += "0\n";
        } else if (r.headers.contains("size") && head.requiredHeaders.at(i).first == "Content-MD5") {
            stringToSign += r.headers.value("size") + "\n";
        }
        else stringToSign += head.requiredHeaders.at(i).second;
    }

    QString date = dateInRFC1123();

    if (r.headers.contains("size") /*&& !r.headers.value("operation").contains("restype")*/) {
       stringToSign += "\nx-ms-blob-type:BlockBlob";
    }
    stringToSign += "\nx-ms-date:" + date;
    stringToSign += "\nx-ms-version:2009-09-19\n";

    stringToSign += "/" + this->storageAccountName + r.headers.value("path");
    if (r.headers.contains("operation")){
        QString temp = r.headers.value("operation");
        stringToSign += "\n" + temp.replace("=", ":").replace("&", "\n");
    }
    QByteArray key = QByteArray::fromBase64(this->storageKey);
    QByteArray hash = HmacSHA::hash(HmacSHA::SHA256, stringToSign, key);

    QString version = "2009-09-19";
    QByteArray test = this->authentication + " " + this->storageAccountName + ":" + hash;

    req.setRawHeader("Authorization", test);
    req.setRawHeader("x-ms-date", date.toUtf8());
    req.setRawHeader("x-ms-version", version.toUtf8());
    if(r.headers.value("verb") == "PUT")  {
        req.setRawHeader("Content-Length", r.headers.value("size").toUtf8());
        req.setRawHeader("x-ms-blob-type", "BlockBlob");
    }

    return req;
}

/*!
  \reimp
  */
void QAzureConnection::setOverrideCloud(bool value) {
    this->overrideCloud = value;
}

/*!
  \reimp
  */

void QAzureConnection::setOverrideLocal(bool value) {
    this->overrideLocal = value;
}

//SEND FUNCTIONS
/*!
  \internal
  */
QNetworkReply* QAzureConnection::sendGet(const QNetworkRequest &req) {
    QEventLoop loop;
    QNetworkReply *reply = manager->get(req);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply;
}

/*!
  \internal
  */
QNetworkReply* QAzureConnection::sendPut(const QNetworkRequest &req, const QByteArray &payload) {
   QEventLoop loop;
   QNetworkReply *reply = manager->put(req, payload);
   connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
   loop.exec();
   return reply;
}

/*!
  \internal
  */
QNetworkReply* QAzureConnection::sendHead(const QNetworkRequest &req) {
    QEventLoop l;
    QNetworkReply *reply = manager->head(req);
    connect(reply, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();
    return reply;
}

//PARSERS
/*!
  \fn QAzureConnection::parseCloudDirListings(QByteArray &contents)
  \brief Parses \a contents and finds Name-tags from the array. Returns the list of names
  */
QList<QString> QAzureConnection::parseCloudDirListings(QByteArray &contents) {
    QXmlStreamReader reader;
    reader.addData(contents);
    QList<QString> foo;
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.name().toString() == "Name") {
            foo.append(reader.readElementText());
        }
    }
    emit finished();
    return foo;
}

