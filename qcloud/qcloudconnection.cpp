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
  and should be checked against. This one parameter version is used to send GET requests to services. \a req is the sent request
  */

/*!
  \fn QCloudConnection::sendPut(const QNetworkRequest &req, const QByteArray &payload)
  this two parameter version of sendData is used to send PUT requests as the data that is sent is located in the payload reference.
  \a req is the constructed request and \a payload is the data
  */

/*!
  \fn QCloudConnection::sendHead(const QNetworkRequest &req)

  sends head-request using constructed \a req
  */


/*!
  \fn QCloudConnection::encode(const Request &r)
  encode function takes an Request-struct \a r and makes the QNetworkRequest object from it. The steps are, create the stringToSign from the
  headers, hash it with HmacSHA1/SHA256, construct the request and return it.
  */


/*!
  \fn QCloudConnection::get(QString bucket, QString fileName)

  Virtual get function that returns the pointer to a QByteArray that is the response from get request.
  Parameters are, \a bucket (the name of the bucket) and \a fileName (the wanted file).
  */

/*!
  \fn QCloudConnection::get(QCloudDir &d)

  \brief get(QCloudDir &d) downloads the whole folder indicated by \a d.

  If setOverrideLocal is true this overwrites every file in the local folder indicated by \a d, else the
  function just gets the files that are not in the local folder.
  */

/*!
  \fn QCloudConnection::put(QCloudFile &f, QString bucket)
  \return true if the operation is success else false
  Virtual put function that retuns true if the put operation is a success, if not false. First parameter
  is the QCloudFile \a file that is put in the cloud and the second parameter is the \a bucket where it is placed.

  Note: Does not check if the overrideCloud is true, so if calling this the file in the cloud is always
        overwriten. If one wants to check overrideCloud then use put(QCloudDir &d)
  */

/*!
   \fn QCloudConnection::put(QCloudDir &d)

   Puts the contents of the CloudDir \a d to the cloud. Takes in account the overrideCloud and overrideLocal
   */
/*!
  \fn QCloudConnection::getCloudDir()
  getCloudDir that returns a list of the names of qclouddirs in the service for a certain account. The account
  is defined by created object.
  */


/*!
  \fn QCloudConnection::getCloudDirContents(QString bucketName)
  \brief getBucketContents retuns a list of names of the files contained in the bucket/container defined by the parameter \a bucketName.
  */

/*!
  \fn QCloudConnection::deleteBlob(QString name, QString bucket)
  Deletes \a name from clouddir \a bucket
  \return true if success else false
  */

/*!
  \fn QCloudConnection::deleteCloudDir(QString bucket)
  Deletes clouddir \a bucket from the cloud
  \return true if success else false
  */

/*!
  \fn QCloudConnection::cloudDirExists(const QString &dirName)
  Returns true if the clouddir \a dirName exists in the cloud, else false
  */

/*!
  \fn QCloudConnection::createCloudDir(const QString &dirName)
  Creates the specified cloudDir with name \a dirName to the cloud, returns true if success else false.
  */

/*!
 * \fn QCloudConnection::setOverrideLocal(bool value)
   \brief setsOverrideLocal to \a value. OverrideLocal defines if the user wants to override files in the local filesystem,
   when getting files from the cloud.
  */

/*!
  \fn QCloudConnection::setOverrideCloud(bool value)
  \brief sets overrideCloud to \a value. Override cloud defines if the user wants to override files in the cloud.
  */
/*!
    \fn void QCloudConnection::finished()
    Emitted when operation is finished.
*/

/*!
    \fn void QCloudConnection::failed()
    Emitted when there was troubles in the operation
*/

/*!
    \fn void QCloudConnection::getCloudDirFinished()
    Emitted when get(QCloudDir &d) finishes.
*/

/*!
    \fn void QCloudConnection::putCloudDirFinished()
    Emitted when put(QCloudDir &d) finishes
*/

/*!
  \fn QCloudConnection::parseCloudDirListings(QByteArray &message)
  \brief should be implemented so that returns the filenames/bucketnames in a list of strings.
  */

/*!
  \fn QCloudConnection::asyncGetCloudFile(QString &bucket, QString &fileName)
  Gets the wanted file \a fileName from \a bucket and returns it inside a qcloudfileresponse
  */
/*!
  \fn QCloudConnection::asyncGetCloudDirContents(QString &cloudDir)
  Gets the contents of a bucket \a cloudDir and returns them inside a qcloudlistresponse
  */
/*!
  \fn QCloudConnection::asyncGetCloudDir()
  Gets contents of the users cloud account and returns them insidea a qcloudlistresponse
  */
