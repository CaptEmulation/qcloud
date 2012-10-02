#ifndef QCLOUDCONNECTION_H
#define QCLOUDCONNECTION_H

#include <QObject>
#include <QHash>
#include <QNetworkReply>

#include "hmacsha.h"
#include "qcloudfile.h"
#include "qcloudresponse.h"

/*!
  \author Jarkko Laitinen
  \version 1.0a
  \interface QCloudConnection

  QCloudConnection is the interface for a cloud connection object. These methods are required and should be
  implemented as documented.
  */
class QCloudDir;
class QCloudConnection : public QObject
{
    Q_OBJECT
public:

    /*!
      Virtual get function that returns the pointer to a QByteArray that is the response from get request.
      Parameters are, bucket(the name of the bucket) and fileName(the wanted file).
      */
    virtual QCloudFile* get(QString bucket, QString fileName) = 0;

    virtual bool get(QCloudDir &d) = 0;
    /*!
      Virtual put function that retuns true if the put operation is a success, if not false. First parameter
      is the QCloudFile that is put in the cloud and the second parameter is the bucket where it is placed.
      */
    virtual bool put(QCloudFile &file, QString bucket) = 0;

    /*!

      */
    virtual bool put(QCloudDir &dir) = 0;

    /*!
      getBuckets that returns a list of the names of buckets/containers in the service for a certain account. The account
      is defined by created object. At the moment there is no possibility to change the account(amz) or storage account(azure)
      after the CloudConnection object has been created.
      */
    virtual QList<QString> getCloudDir() = 0;

    /*!
      getBucketContents retuns a list of names of the files contained in the bucket/container defined by the parameter bucketName.
      */
    virtual QList<QString> getCloudDirContents(QString bucketName) = 0;

    /*!
      The public struct for request is used in conjunction with the encode function. As the implementations differ in both implementations
      QAzure and QAmazon, one should look at their documentation for the minimal headers.
      */
    struct Request {
        QHash<QString, QString> headers;
    };

    /*!
      Deletes mentioned blob from the wanted bucket.
      */
    virtual bool deleteBlob(QString name, QString bucket) = 0;

    /*!
      Deletes bucket from the account.
      */
    virtual bool deleteCloudDir(QString bucket) = 0;


    virtual QList<QString> parseCloudDirListings(QByteArray &data) = 0;

    /*!
      Returns true if the dir exists in the cloud, else false
      */
    virtual bool cloudDirExists(const QString &dirName) = 0;

    /*!
      Creates the specified cloudDir to the cloud, returns true if success else false.
      */
    virtual bool createCloudDir(const QString &dirName) = 0;

    virtual void setOverrideLocal(bool value) = 0;
    virtual void setOverrideCloud(bool value) = 0;
protected:

    QCloudConnection();

private:
    /*!
      sendData uses QNetworkAccessManager to send and receive data from the cloud. Errors in the request are inside the QNetworkReply
      and should be checked against. This one parameter version is used to send GET requests to services.
      */
    virtual QNetworkReply* sendGet(const QNetworkRequest &req) = 0;

    /*!
      this two parameter version of sendData is used to send PUT requests as the data that is sent is located in the payload reference.
      */
    virtual QNetworkReply* sendPut(const QNetworkRequest &req, const QByteArray &payload) = 0;

    /*!

      */
    virtual QNetworkReply* sendHead(const QNetworkRequest &req) = 0;

    /*!
      encode function takes an Request-struct and makes the QNetworkRequest object from it. The steps are, create the stringToSign from the
      headers, hash it with HmacSHA1/SHA256, construct the request and return it.
      */
    virtual QNetworkRequest encode(const Request &r) = 0;

    virtual QCloudResponse::RESPONSETYPE findType(QNetworkReply &reply, QByteArray &contents) = 0;



signals:
    void requestFinished();
    void finished();
    void getCloudDirFinished();
    void putCloudDirFinished();
    void cloudRequestFinished(QCloudResponse*);
};

#endif // QCLOUDCONNECTION_H
