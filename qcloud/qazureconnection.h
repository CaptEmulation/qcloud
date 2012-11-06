#ifndef QAZURECONNECTION_H
#define QAZURECONNECTION_H

#include "qcloudconnection.h"
#include "qclouditem.h"
#include <QEventLoop>
#include <QXmlStreamReader>
#include "qcloudresponse.h"
#include "qclouddir.h"
#include <QNetworkReply>
#include "qcloudfileresponse.h"
#include "qcloudlistresponse.h"

class QAzureConnection : public QCloudConnection
{
    Q_OBJECT

public:
    ~QAzureConnection();
    QAzureConnection(QByteArray url, QByteArray authentication, QByteArray storageKey);


    virtual bool put(QCloudFile &f, QString bucket);
    virtual bool put(QCloudDir &dir);
    virtual QList<QString> getCloudDir();
    virtual QList<QString> getCloudDirContents(QString bucketName);
    virtual bool deleteBlob(QString name, QString bucket);
    virtual bool deleteCloudDir(QString bucket);

    virtual QCloudFile* get(QString bucket, QString name);
    virtual bool get(QCloudDir &d);
    virtual bool cloudDirExists(const QString &dirName);
    virtual  bool createCloudDir(const QString &dirName);

    virtual QCloudFileResponse* asyncGetCloudFile(QString &bucket, QString &fileName);
    virtual QCloudListResponse* asyncGetCloudDir();
    virtual QCloudListResponse* asyncGetCloudDirContents(QString &cloudDir);

private:
    virtual QNetworkRequest encode(const Request &r);
    virtual QNetworkReply* sendPut(const QNetworkRequest &req, const QByteArray &payload);
    virtual QNetworkReply* sendGet(const QNetworkRequest &req);
    virtual QNetworkReply* sendHead(const QNetworkRequest &req);
    virtual QList<QString> parseCloudDirListings(QByteArray &contents);
    QList<QString> parseCloudDirContentsListing(QByteArray &reply);


    /**
      Azure requires dateTimes in RFC1123 format that is not supported by Qt's QDateTime at the moment. This helperfunction
      does the time string needed and returns it.
      */
    QString dateInRFC1123();

    /*!

      \struct
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
};

#endif // QAZURECONNECTION_H
