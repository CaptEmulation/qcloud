#ifndef QCLOUDCONNECTION_H
#define QCLOUDCONNECTION_H

#include <QObject>
#include <QHash>
#include <QNetworkReply>
#include "hmacsha.h"
#include "qcloudfile.h"
#include "qcloudresponse.h"

class QCloudDir;

class QCloudConnection : public QObject
{
    Q_OBJECT
public:
    struct Request {
        QHash<QString, QString> headers;
    };

    virtual QCloudFile* get(QString bucket, QString fileName) = 0;
    virtual bool get(QCloudDir &d) = 0;
    virtual bool put(QCloudFile &file, QString bucket) = 0;
    virtual bool put(QCloudDir &dir) = 0;
    virtual QList<QString> getCloudDir() = 0;
    virtual QList<QString> getCloudDirContents(QString bucketName) = 0;
    virtual bool deleteBlob(QString name, QString bucket) = 0;
    virtual bool deleteCloudDir(QString bucket) = 0;
    virtual QList<QString> parseCloudDirListings(QByteArray &data) = 0;
    virtual bool cloudDirExists(const QString &dirName) = 0;
    virtual bool createCloudDir(const QString &dirName) = 0;
    virtual void setOverrideLocal(bool value) = 0;
    virtual void setOverrideCloud(bool value) = 0;

protected:
    QCloudConnection();

private:
    virtual QNetworkReply* sendGet(const QNetworkRequest &req) = 0;
    virtual QNetworkReply* sendPut(const QNetworkRequest &req, const QByteArray &payload) = 0;
    virtual QNetworkReply* sendHead(const QNetworkRequest &req) = 0;
    virtual QNetworkRequest encode(const Request &r) = 0;

signals:
    void requestFinished();
    void finished();
    void failed();
    void getCloudDirFinished();
    void putCloudDirFinished();
    void cloudRequestFinished(QCloudResponse*);
};

#endif // QCLOUDCONNECTION_H
