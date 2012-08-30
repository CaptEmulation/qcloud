#ifndef QAZURECONNECTION_H
#define QAZURECONNECTION_H

#include "qcloudconnection.h"
#include "qclouditem.h"
#include <QEventLoop>
#include <QXmlStreamReader>

/**
  Author: Jarkko Laitinen

  Implementation of the QCloudConnection interface for Microsoft Windows Azure.
  */
class QAzureConnection : public QCloudConnection
{
    Q_OBJECT

public:
    ~QAzureConnection();

    /**
      Constructor to create new QAzureConnections. This contains three parameters and all should be in the right format.
      The first parameter is QByteArray containing the url of the service i.e. "http://kikkare.blob.core.windows.net" where
      kikkare is the storage accounts name. Authentication contains either SharedKey or SharedKeyLite. At the moment, this
      library has better support for SharedKey authentication. storageKey parameter should be made with QByteArray::fromBase64()
      as the key provided by Microsoft is Base64 encoded. The constructor also creates a new Headers-struct according to the
      provided authentication type.
      */
    QAzureConnection(QByteArray url, QByteArray authentication, QByteArray storageKey);

    /**
      Documentation for the inherited functions can be found from QCloudConnection.
      */
    virtual QByteArray* get(QString name);

    /**
      At the moment this is nofunc as it always returns false.
      */
    virtual bool put(QCloudFile &f, QString bucket);

    /**
      put(QCloudTable) is nofunc at the moment.
      */
    virtual bool put(QCloudTable &table);
    virtual bool put(QByteArray &array, QString fileName, QString bucket);
    virtual QList<QString> getBuckets();
    virtual QList<QString> getBucketContents(QString bucketName);
    virtual bool deleteBlob(QString name, QString bucket);
    virtual bool deleteBucket(QString bucket);

    virtual QByteArray* get(QString bucket, QString name);

    /**
      A helperfunction to ease the creation of containers in Azure.
      */
    bool createContainer(QString name);

private:
    virtual QNetworkRequest encode(const Request &r);
    virtual QNetworkReply* sendData(const QNetworkRequest &req, const QByteArray &payload);
    virtual QNetworkReply* sendData(const QNetworkRequest &req);

    QList<QString> parseBucketListing(QByteArray &contents);
    QList<QString> parseBucketContentsListing(QByteArray &reply);

    /**
      Azure requires dateTimes in RFC1123 format that is not supported by Qt's QDateTime at the moment. This helperfunction
      does the time string needed and returns it.
      */
    QString dateInRFC1123();

    /**
      Headers struct to help the formation of stringToSigns. Azure requires 12 parameters if using SharedKey and 5 if using
      SharedKeyLite. This needs to be initialized according the rest specification of Azure, but when using this class
      there should be no need to tinker with this.
      */
    struct Headers {
        QList< QPair<QString, QString> > requiredHeaders;
    };

    /**
      Initializes the Headers struct according the needed header types
      */
    void initializeHeaders();
    void initializeSharedKeyLiteHeaders();

    Headers head;

    QNetworkAccessManager *manager;
    QByteArray url;
    QByteArray authentication;
    QByteArray storageKey;
};

#endif // QAZURECONNECTION_H
