#ifndef QAMAZONCONNECTION_H
#define QAMAZONCONNECTION_H

#include "qcloudconnection.h"
#include <QString>
#include <QHash>
#include <QXmlStreamReader>
#include "qcloudfile.h"

class QAmazonConnection : public QCloudConnection
{
    Q_OBJECT
public:
    /**
      Deconstructor, deletes allocated objects
     */
    ~QAmazonConnection();

    /**
      Constructor, parameters are as follows: host = the adress where the requests are sent, i.e. s3.amazonaws.com
                                              user = username in the service
                                              password = AWSAccessKeyId
                                              secret = the secret key obtained from the service
      */
    QAmazonConnection(QByteArray host, QByteArray user, QByteArray password, QByteArray secret);

    /**
      The next virtual functions are inherited from QCloudConnection. These function as detailed in QCloudConnection
      but differences are detailed here.
      */
    virtual QByteArray* get(QString bucket, QString fileName);
    virtual bool put(QCloudFile &f, QString bucket);
    virtual bool put(QCloudTable &table);
    virtual bool put(QByteArray &array, QString fileName, QString bucket);
    virtual QList<QString> getBuckets();
    virtual QList<QString> getBucketContents(QString bucketName);
    virtual bool deleteBlob(QString name, QString bucket);
    virtual bool deleteBucket(QString bucket);
private:

    virtual QNetworkRequest encode(const Request &r);
    virtual QNetworkReply* sendData(const QNetworkRequest &req);
    virtual QNetworkReply* sendData(const QNetworkRequest &req, const QByteArray &payload);

    /**
      Amazon defines that + and / should be replaced from the hashed to %2F and %2B. This function
      takes a pointer to the array and replaces the occurances.
      */
    void replaceUnallowed(QByteArray *array);

    /**
      Parser method that returns a list of strings that contain the files included in the getBucketContents()
      response.
      */
    QList<QString> parseBucketContentListing(QByteArray *array);

    /**
      Parser method that uses QXmlStreamReader to find the buckets from getBuckets() listing. Returns list of the
      buckets in the users account.
      */
    QList<QString> parseBucketListings(QByteArray* message);

    QNetworkAccessManager *manager;
    QByteArray host;
    QByteArray username;
    QByteArray password;
    QByteArray secret;

};

#endif // QAMAZONCONNECTION_H
