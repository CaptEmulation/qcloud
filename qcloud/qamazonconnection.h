#ifndef QAMAZONCONNECTION_H
#define QAMAZONCONNECTION_H

#include "qcloudconnection.h"
#include <QString>
#include <QHash>
#include <QXmlStreamReader>
#include "qcloudfile.h"
#include "qcloudresponse.h"
#include "qclouddir.h"
#include <QTimer>

class QAmazonConnection : public QCloudConnection
{
    Q_OBJECT
public:
    ~QAmazonConnection();


    QAmazonConnection(QByteArray user, QByteArray password, QByteArray secret);

    /**
      The next virtual functions are inherited from QCloudConnection. These function as detailed in QCloudConnection
      but differences are detailed here.
      */
    virtual QCloudFile* get(QString bucket, QString fileName);
    virtual bool get(QCloudDir &d);
    virtual bool put(QCloudFile &f, QString bucket);
    virtual bool put(QCloudDir &d);
    virtual QList<QString> getCloudDir();
    virtual QList<QString> getCloudDirContents(QString bucketName);
    virtual bool deleteBlob(QString name, QString bucket);
    virtual bool deleteCloudDir(QString bucket);

    /*!
      \fn QAmazonConnection::parseCloudDirContentListing()
      \brief Parses message received from cloud and returns a list of blobs in a clouddir.

      Parser method that returns a list of strings that contain the files included in the getBucketContents()
      response.
      */
    QList<QString> parseCloudDirContentListing(QByteArray *array);

    /*!
      \fn QAmazonConnection::parseCLoudDirListing
      \brief Takes an Bytearray containing the reply from cloud and returns a list of qclouddirs in the cloud.

      Parser method that uses QXmlStreamReader to find the buckets from getBuckets() listing. Returns list of the
      buckets in the users account.
      */
    QList<QString> parseCloudDirListings(QByteArray &message);
    virtual bool createCloudDir(const QString &dirName);
    virtual bool cloudDirExists(const QString &dirName);

private:

    virtual QNetworkRequest encode(const Request &r);
    virtual QNetworkReply* sendGet(const QNetworkRequest &req);
    virtual QNetworkReply* sendPut(const QNetworkRequest &req, const QByteArray &payload);
    virtual QNetworkReply* sendHead(const QNetworkRequest &req);

    /*!
      \fn QAmazonConnection::replaceUnallowed
      \brief replaces / and + with their encoded values.

      Amazon defines that + and / should be replaced from the hashed to %2F and %2B. This function
      takes a pointer to the array and replaces the occurances.
      */
    void replaceUnallowed(QByteArray *array);



    /*!
      \fn QAmazonConnection::findType
      \brief Finds the type of request from the reply

      Finds the type of the reply from the reply and places it contents to contents, does not delete the QNetworkReply
      */
    virtual QCloudResponse::RESPONSETYPE findType(QNetworkReply &reply, QByteArray &contents);

    QNetworkAccessManager *manager;
    QByteArray host;
    QByteArray username;
    QByteArray password;
    QByteArray secret;

    virtual void setOverrideLocal(bool value);
    virtual void setOverrideCloud(bool value);

    bool overrideLocal;
    bool overrideCloud;

signals:
    void valueChanged(int);
    void setRange(int,int);

private slots:
    virtual void requestFinished(QNetworkReply*);

};

#endif // QAMAZONCONNECTION_H
