#include "qcloudconnection.h"

QCloudConnection::QCloudConnection(){}

/*!
  The public struct for request is used in conjunction with the encode function. As the implementations differ in both implementations
  QAzure and QAmazon, one should look at their documentation for the minimal headers.
  */
struct Request {
    QHash<QString, QString> headers;
};


/*!
  sendData uses QNetworkAccessManager to send and receive data from the cloud. Errors in the request are inside the QNetworkReply
  and should be checked against. This one parameter version is used to send GET requests to services.
  */
QNetworkReply* sendGet(const QNetworkRequest &req);

/*!
  this two parameter version of sendData is used to send PUT requests as the data that is sent is located in the payload reference.
  */
QNetworkReply* sendPut(const QNetworkRequest &req, const QByteArray &payload);

/*!

  */
QNetworkReply* sendHead(const QNetworkRequest &req);

/*!
  encode function takes an Request-struct and makes the QNetworkRequest object from it. The steps are, create the stringToSign from the
  headers, hash it with HmacSHA1/SHA256, construct the request and return it.
  */
QNetworkRequest encode(const Request &r);


QCloudResponse::RESPONSETYPE findType(QNetworkReply &reply, QByteArray &contents);


/*!
  \fn get(QString bucket, QString fileName)

  Virtual get function that returns the pointer to a QByteArray that is the response from get request.
  Parameters are, bucket(the name of the bucket) and fileName(the wanted file).
  */
QCloudFile* get(QString bucket, QString fileName);

bool get(QCloudDir &d);
/*!
  \fn virtual bool get(QCloudDir &d)

  Virtual put function that retuns true if the put operation is a success, if not false. First parameter
  is the QCloudFile that is put in the cloud and the second parameter is the bucket where it is placed.
  */
bool put(QCloudFile &file, QString bucket);

/*!

  */
bool put(QCloudDir &dir);

/*!
  getBuckets that returns a list of the names of buckets/containers in the service for a certain account. The account
  is defined by created object. At the moment there is no possibility to change the account(amz) or storage account(azure)
  after the CloudConnection object has been created.
  */
QList<QString> getCloudDir();

/*!
  getBucketContents retuns a list of names of the files contained in the bucket/container defined by the parameter bucketName.
  */
QList<QString> getCloudDirContents(QString bucketName);


/*!
  Deletes mentioned blob from the wanted bucket.
  */
bool deleteBlob(QString name, QString bucket);

/*!
  Deletes bucket from the account.
  */
bool deleteCloudDir(QString bucket);


QList<QString> parseCloudDirListings(QByteArray &data);

/*!
  Returns true if the dir exists in the cloud, else false
  */
bool cloudDirExists(const QString &dirName);

/*!
  Creates the specified cloudDir to the cloud, returns true if success else false.
  */
bool createCloudDir(const QString &dirName);

void setOverrideLocal(bool value);
void setOverrideCloud(bool value);
