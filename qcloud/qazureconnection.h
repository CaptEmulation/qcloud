#ifndef QAZURECONNECTION_H
#define QAZURECONNECTION_H

#include "qcloudconnection.h"
#include "qclouditem.h"
#include <QEventLoop>
#include <QXmlStreamReader>
#include "qcloudresponse.h"
#include "qclouddir.h"
#include <QNetworkReply>

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
      At the moment this is nofunc as it always returns false.
      */
    virtual bool put(QCloudFile &f, QString bucket);

    /**
      put(QCloudTable) is nofunc at the moment.
      */
    virtual bool put(QCloudTable &table);
    virtual bool put(QCloudDir &dir);
    virtual QList<QString> getCloudDir();
    virtual QList<QString> getCloudDirContents(QString bucketName);
    virtual bool deleteBlob(QString name, QString bucket);
    virtual bool deleteCloudDir(QString bucket);


    virtual QCloudFile* get(QString bucket, QString name);
    virtual bool get(QCloudDir &d);
    virtual bool cloudDirExists(const QString &dirName);
    virtual  bool createCloudDir(const QString &dirName);

private:

    virtual QNetworkRequest encode(const Request &r);
    virtual QNetworkReply* sendPut(const QNetworkRequest &req, const QByteArray &payload);
    virtual QNetworkReply* sendGet(const QNetworkRequest &req);
    virtual QNetworkReply* sendHead(const QNetworkRequest &req);
    virtual QList<QString> parseCloudDirListings(QByteArray &contents);
    QList<QString> parseCloudDirContentsListing(QByteArray &reply);

    /**
      A helperfunction to ease the creation of containers in Azure.
      */


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

    virtual QCloudResponse::RESPONSETYPE findType(QNetworkReply &reply, QByteArray &contents);

    virtual void setOverrideLocal(bool value);
    virtual void setOverrideCloud(bool value);

    Headers head;

    QNetworkAccessManager *manager;
    QByteArray url;
    QByteArray storageAccountName;
    QByteArray storageKey;
    QByteArray authentication;

    bool overrideLocal;
    bool overrideCloud;

signals:
    void valueChanged(int);
    void setRange(int,int);

private slots:
    virtual void requestFinished(QNetworkReply*);
    virtual void getCloudDirFinished(QNetworkReply*);
    virtual void getCloudDirContentsFinished(QNetworkReply*);
    virtual void getCloudFileFinished(QNetworkReply*);
};

#endif // QAZURECONNECTION_H
