#include "qcloudconnection.h"

/*!
 \class QCloudConnection
 \brief Cloud connection API.

  QCloudConnection is the interface for a cloud connection object. These methods are required and should be
  implemented as documented.
 */

/*!
 * Constructor
 */
QCloudConnection::QCloudConnection()
{

}


/*!
  \fn QCloudConnection::sendGet(const QNetworkRequest &req)
  sendData uses QNetworkAccessManager to send and receive data from the cloud. Errors in the request are inside the QNetworkReply
  and should be checked against. This one parameter version is used to send GET requests to services.
  */

/*!
  \fn QCloudConnection::sendPut(const QNetworkRequest &req, const QByteArray &payload)
  this two parameter version of sendData is used to send PUT requests as the data that is sent is located in the payload reference.
  */

/*!
  \fn QCloudConnection::sendHead(const QNetworkRequest &req)
  */


/*!
  \fn QCloudConnection::encode(const Request &r)
  encode function takes an Request-struct and makes the QNetworkRequest object from it. The steps are, create the stringToSign from the
  headers, hash it with HmacSHA1/SHA256, construct the request and return it.
  */


/*!
  \fn QCloudConnection::get(QString bucket, QString fileName)

  Virtual get function that returns the pointer to a QByteArray that is the response from get request.
  Parameters are, bucket(the name of the bucket) and fileName(the wanted file).
  */

/*!
  \fn QCloudConnection::get(QCloudDir &d)

  TODO
  */

/*!
  \fn QCloudConnection::put(QCloudFile &file, QString bucket)

  Virtual put function that retuns true if the put operation is a success, if not false. First parameter
  is the QCloudFile that is put in the cloud and the second parameter is the bucket where it is placed.
  */

/*!
  \fn QCloudConnection::getCloudDir()
  getBuckets that returns a list of the names of buckets/containers in the service for a certain account. The account
  is defined by created object. At the moment there is no possibility to change the account(amz) or storage account(azure)
  after the CloudConnection object has been created.
  */


/*!
  \fn QCloudConnection::getCloudDirContents(QString bucketName)
  getBucketContents retuns a list of names of the files contained in the bucket/container defined by the parameter bucketName.
  */

/*!
  \fn QCloudConnection::deleteBlob(QString name, QString bucket)
  Deletes mentioned blob from the wanted bucket.
  */

/*!
  \fn QCloudConnection::deleteCloudDir(QString bucket)
  Deletes bucket from the account.
  */

/*!
  \fn QCloudConnection::cloudDirExists(const QString &dirName)
  Returns true if the dir exists in the cloud, else false
  */

/*!
  \fn QCloudConnection::createCloudDir(const QString &dirName)
  Creates the specified cloudDir to the cloud, returns true if success else false.
  */

/*!
 * \fn QCloudConnection::setOverrideLocal(bool value)
 * lorem impsum \a value
  */

/*!
    \fn void QCloudConnection::requestFinished()
    Emitted when xxx
*/

/*!
    \fn void QCloudConnection::finished()
    Emitted when xxx
*/

/*!
    \fn void QCloudConnection::failed()
    Emitted when xxx
*/

/*!
    \fn void QCloudConnection::getCloudDirFinished()
    Emitted when xxx
*/

/*!
    \fn void QCloudConnection::putCloudDirFinished()
    Emitted when xxx
*/

/*!
    \fn void QCloudConnection::cloudRequestFinished(QCloudResponse*)
    Emitted when xxx
*/
