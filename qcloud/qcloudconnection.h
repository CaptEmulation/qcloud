#ifndef QCLOUDCONNECTION_H
#define QCLOUDCONNECTION_H

#include <QObject>
#include <QHash>
#include <QNetworkReply>
#include "hmacsha.h"

#include "qcloudbucket.h"
#include "qcloudfile.h"
#include "qcloudtable.h"

/**
  Author: Jarkko Laitinen
  Version: 1.0a


  QCloudConnection is the interface for a cloud connection object. These methods are required and should be
  implemented as documented.
  */

class QCloudConnection : public QObject
{
    Q_OBJECT
public:
    /**
      Constructor for the class, that is not meant to be used, and it cant as every method is virtual.
      */
    QCloudConnection();

    /**
      Virtual get function that returns the pointer to a QByteArray that is the response from get request.
      Parameters are, bucket(the name of the bucket) and fileName(the wanted file).
      */
    virtual QByteArray* get(QString bucket, QString fileName) = 0;

    /**
      Virtual put function that retuns true if the put operation is a success, if not false. First parameter
      is the QCloudFile that is put in the cloud and the second parameter is the bucket where it is placed.
      */
    virtual bool put(QCloudFile &file, QString bucket) = 0;

    /**
      Another virtual put method, but this time for QCloudTables. At the moment is returns false in all of the
      implementations (QAmazon and QAzureConnection).
      */
    virtual bool put(QCloudTable &table) = 0;

    /**
      Put method that takes three parameters, the contents that is wanted to put in the cloud, filename of the file
      and bucket where it should be placed.
      */
    virtual bool put(QByteArray &array, QString fileName, QString bucket) = 0;

    /**
      getBuckets that returns a list of the names of buckets/containers in the service for a certain account. The account
      is defined by created object. At the moment there is no possibility to change the account(amz) or storage account(azure)
      after the CloudConnection object has been created.
      */
    virtual QList<QString> getBuckets() = 0;

    /**
      getBucketContents retuns a list of names of the files contained in the bucket/container defined by the parameter bucketName.
      */
    virtual QList<QString> getBucketContents(QString bucketName) = 0;

    /**
      The public struct for request is used in conjunction with the encode function. As the implementations differ in both implementations
      QAzure and QAmazon, one should look at their documentation for the minimal headers.
      */
    struct Request {
        QHash<QString, QString> headers;
    };
private:
    /**
      sendData uses QNetworkAccessManager to send and receive data from the cloud. Errors in the request are inside the QNetworkReply
      and should be checked against. This one parameter version is used to send GET requests to services.
      */
    virtual QNetworkReply* sendData(const QNetworkRequest &req) = 0;

    /**
      this two parameter version of sendData is used to send PUT requests as the data that is sent is located in the payload reference.
      */
    virtual QNetworkReply* sendData(const QNetworkRequest &req, const QByteArray &payload) = 0;

    /**
      encode function takes an Request-struct and makes the QNetworkRequest object from it. The steps are, create the stringToSign from the
      headers, hash it with HmacSHA1/SHA256, construct the request and return it.
      */
    virtual QNetworkRequest encode(const Request &r) = 0;
};

#endif // QCLOUDCONNECTION_H
