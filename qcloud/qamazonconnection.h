#ifndef QAMAZONCONNECTION_H
#define QAMAZONCONNECTION_H

#include "qcloudconnection.h"
#include <QString>
#include <QHash>
#include <QXmlStreamReader>
#include "qcloudfile.h"
#include "qcloudresponse.h"
#include "qclouddir.h"
#include "qcloudlistresponse.h"
#include "qcloudfileresponse.h"
#include <QTimer>

class QAmazonConnection : public QCloudConnection
{
    Q_OBJECT

public:
    QAmazonConnection(QByteArray user, QByteArray password, QByteArray secret);
    ~QAmazonConnection();
    virtual QCloudFile* get(QString bucket, QString fileName);
    virtual bool get(QCloudDir &d);
    virtual bool put(QCloudFile &f, QString bucket);
    virtual bool put(QCloudDir &d);
    virtual QList<QString> getCloudDir();
    virtual QList<QString> getCloudDirContents(QString bucketName);
    virtual bool deleteBlob(QString name, QString bucket);
    virtual bool deleteCloudDir(QString bucket);
    virtual QCloudFileResponse* asyncGetCloudFile(QString &bucket, QString &fileName);
    virtual QCloudListResponse* asyncGetCloudDir();
    virtual QCloudListResponse* asyncGetCloudDirContents(QString &cloudDir);
    QList<QString> parseCloudDirContentListing(QByteArray *array);
    QList<QString> parseCloudDirListings(QByteArray &message);
    virtual bool createCloudDir(const QString &dirName);
    virtual bool cloudDirExists(const QString &dirName);

private:

    virtual QNetworkRequest encode(const Request &r);
    virtual QNetworkReply* sendGet(const QNetworkRequest &req);
    virtual QNetworkReply* sendPut(const QNetworkRequest &req, const QByteArray &payload);
    virtual QNetworkReply* sendHead(const QNetworkRequest &req);

    void replaceUnallowed(QByteArray *array);

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

};

#endif // QAMAZONCONNECTION_H
