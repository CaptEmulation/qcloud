#include "qamazonconnection.h"
#include <QDebug>
#include <QSslSocket>

#include <QDateTime>
#include <QEventLoop>

/*!
  \class QAmazonConnection
  \brief Implementation of the QCloudConnection interface for Amazon.

  Constructor, parameters are as follows: host = the adress where the requests are sent, i.e. s3.amazonaws.com
                                          user = username in the service
                                          password = AWSAccessKeyId
                                          secret = the secret key obtained from the service

  \sa QCloudConnection
 */

/*!
 * Constructor
  \a user username to the service
  \a password awsaccesskeyid
  \a secret secretkey
 */
QAmazonConnection::QAmazonConnection(QByteArray user, QByteArray password, QByteArray secret) {
    this->host = "s3.amazonaws.com";
    this->username = user;
    this->password = password;
    this->secret = secret;
    manager = new QNetworkAccessManager(this);
}

/*!
 * Destructor
 */
QAmazonConnection::~QAmazonConnection() {
    manager->deleteLater();
}

/*!
  \brief Deletes a blob( \a name ) in a bucket ( \a bucket), does not work
  */
bool QAmazonConnection::deleteBlob(QString name, QString bucket) {
    Request r;
    r.headers.insert("verb", "DELETE");
    r.headers.insert("bucket", bucket);
    r.headers.insert("filename", name);
    return true;
}

/*!
  \brief Deletes a whole bucket (\a bucket), does not work
  */
bool QAmazonConnection::deleteCloudDir(QString bucket) {
    Request r;
    r.headers.insert("verb", "DELETE");
    r.headers.insert("bucket", bucket);
    return true;
}

/*!
  \brief Creates a new clouddir named \a dirName to the cloud. Returns true if success
  else false.
  */
bool QAmazonConnection::createCloudDir(const QString &dirName) {
    Request r;
    r.headers.insert("verb", "PUT");
    r.headers.insert("bucket", dirName);
    r.headers.insert("filesize", "0");

    QByteArray data("");
    QNetworkReply *reply = sendPut(encode(r), data);
    r.headers.clear();

    if (reply->error() != 0) {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
        reply->deleteLater();
        return false;
    }
    reply->deleteLater();
    qDebug() << reply->readAll();
    return true;
}

/*!
  \brief Checks if the \a dirName exists in the cloud.
  */
bool QAmazonConnection::cloudDirExists(const QString &dirName) {
    Request r;
    r.headers.insert("verb", "HEAD");
    r.headers.insert("bucket", dirName);
    QNetworkReply *reply = sendHead(encode(r));
    if (reply->error() != 0) {
       return false;
       emit cloudError();
    }
    return true;
}

/*!
  \brief Puts a \a d  clouddir to the cloud. If overrideCloud is defined as true
  the whole directory is overridden else only the new files are uploaded.
  */
bool QAmazonConnection::put(QCloudDir &d) {
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

    return true;
}

/*!
  \brief Downloads clouddir \a d to the computer.

  If overrideLocal is true the function downloads everything and overrides local copies,
  else it just gets the files that do not exist in the computer.
  */
bool QAmazonConnection::get(QCloudDir &d) {

    QString path = d.getPath();
    QList<QString> files = getCloudDirContents(path);
    int size = files.size();
    emit setRange(0, size);

    QList<QString> localFiles = d.getCloudDirContentsAsString();

    if (!overrideLocal && localFiles.size() > 0) {
        for (int i = 0; i < size; i++) {
            if (!localFiles.contains(files.at(i))) {
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
    return true;
}

/*!
  \brief Downloads the file \a fileName from a bucket \a bucket. Returns a pointer to a new
  QCloudFile that is also a local file.
  */
QCloudFile* QAmazonConnection::get(QString bucket, QString fileName) {
    Request r;

    r.headers.insert("verb", "GET");
    r.headers.insert("bucket", bucket);
    r.headers.insert("filename", fileName);


    QNetworkReply *reply = sendGet(encode(r));
    if (reply->error() != 0) {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
        qDebug() << "error from get";
        emit failed();
        return new QCloudFile("", fileName, bucket);
    }
    QCloudFile* file = new QCloudFile(reply->readAll(), fileName, bucket);
    reply->deleteLater();
    return file;

}

/*!
 \brief getCloudDir gets the list of buckets in the cloud owned by the creator.
  */
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

/*!
  \brief Gets the contents of a CloudDir \a bucketName as a list of strings.
  */
QList<QString> QAmazonConnection::getCloudDirContents(QString bucketName) {
    Request r;
    QNetworkReply *reply;

    r.headers.insert("verb", "GET");
    r.headers.insert("bucket", bucketName);

    reply = sendGet(encode(r));
    if (reply->error() != 0) {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
    }
    QByteArray array = reply->readAll();
    reply->deleteLater();
    return parseCloudDirContentListing(&array);
}


/*!
  \internal
  \brief replaces / and + with their encoded values.

  Amazon defines that + and / should be replaced from the hashed to %2F and %2B. This function
  takes a pointer to the array and replaces the occurances.
  */
void QAmazonConnection::replaceUnallowed(QByteArray *array) {
    array->replace('/', "%2F");
    array->replace('+', "%2B");
}

/*!
  \brief Puts the CloudFile \a f to bucket \a bucket
  */
bool QAmazonConnection::put(QCloudFile &f, QString bucket) {
    Request r;
    r.headers.insert("verb","PUT");
    r.headers.insert("filename", f.getName());
    r.headers.insert("bucket", bucket);
    r.headers.insert("Content-Type", "text/plain");
    r.headers.insert("filesize", QByteArray::number(f.getSize()));


    QNetworkReply *reply = sendPut(encode(r), f.getContents());
    reply->deleteLater();
    if (reply->error() != 0) {
        qDebug() << reply->readAll();
        emit cloudError();
        return false;
    }
    emit finished();
    return true;
}

/**
  \internal
  \brief Creates the request that is then sent to Amazon, everything should be as general as possible and
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

    QByteArray hashedSignature = HmacSHA::hash(HmacSHA::SHA1, stringToSign, this->secret);


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

/*!
  \brief Sets the overrideCloud variable to \a value
  */
void QAmazonConnection::setOverrideCloud(bool value) {
    this->overrideCloud = value;
}

/*!
  \brief Sets the overrideLocal variabl to \a value
  */
void QAmazonConnection::setOverrideLocal(bool value) {
    this->overrideLocal = value;
}

/*!
  \brief Sends put-request \a r with payload \a payload

  The internal implemenation has QEventLoop running until the reply
  is done.

  */
QNetworkReply* QAmazonConnection::sendPut(const QNetworkRequest &r, const QByteArray &payload) {
    QEventLoop l;
    QNetworkReply *reply = manager->put(r, payload);
    connect(reply, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();

    return reply;
}

/*!
  \brief Sends get-request \a r

  The internal implemenation has QEventLoop running until the reply
  is done.
  */
QNetworkReply* QAmazonConnection::sendGet(const QNetworkRequest &r) {
    QEventLoop l;
    QNetworkReply *reply = manager->get(r);
    connect(reply, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();

    return reply;
}

/*!
  \brief Sends head-request \a r

  The internal implemenation has QEventLoop running until the reply
  is done.
  */
QNetworkReply* QAmazonConnection::sendHead(const QNetworkRequest &r) {
    QEventLoop l;
    QNetworkReply *reply = manager->head(r);
    connect(reply, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();
    return reply;
}

/**
  \brief Parses the \a message to a list of strings.


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

/*!
  \brief Parses message received from cloud \a message and returns a list of blobs in a clouddir.

  Parser method that returns a list of strings that contain the files included in the getBucketContents()
  response.
  */
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

/*!
  \brief Does getCloudDir, but without using the send* functions. Returns pointer to the
  new list response.

  Does not use QEventLoop
  */
QCloudListResponse* QAmazonConnection::asyncGetCloudDir() {
    Request r;
    QNetworkReply *reply;
    QNetworkRequest req;
    r.headers.insert("verb", "GET");
    req = encode(r);

    reply = manager->get(req);

    return new QCloudListResponse(reply);
}

/*!
  \brief Gets the contents of the \a cloudDir

  Does not use QEventLoop
  */
QCloudListResponse* QAmazonConnection::asyncGetCloudDirContents(QString &cloudDir) {
    Request r;
    QNetworkReply *reply;
    QNetworkRequest req;
    r.headers.insert("verb", "GET");
    r.headers.insert("bucket", cloudDir);
    req = encode(r);
    reply = manager->get(req);
    return new QCloudListResponse(reply);
}

/*!
  \brief Gets the file \a fileName from the bucket \a bucket.

  Does not use QEventLoop
  */
QCloudFileResponse* QAmazonConnection::asyncGetCloudFile(QString &bucket, QString &fileName) {
    Request r;
    QNetworkReply *reply;
    QNetworkRequest req;

    r.headers.insert("verb", "GET");
    r.headers.insert("bucket", bucket);
    r.headers.insert("filename", fileName);
    req = encode(r);
    reply = manager->get(req);
    return new QCloudFileResponse(reply);
}

/*!
  \fn QAmazonConnection::cloudError()
  \brief Is emitted when there went something wrong with the transfer
  */

/*!
  \fn QAmazonConnection::finished()
  \brief Is emitted when the operation has finished
 */

/*!
  \fn QAmazonConnection::setRange()
  \brief is emitted when many files are downloaded at once. 
  */

/*!
  \fn QAmazonConnection::valueChanged()
  \brief valueChanged() is emitted when a file has been downloaded. This way the GUI can be updated according.
  */
