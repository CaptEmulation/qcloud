#ifndef QAZURECONNECTION_H
#define QAZURECONNECTION_H

#include "qcloudconnection.h"
#include "qclouditem.h"
#include <QEventLoop>
#include <QXmlStreamReader>

class QAzureConnection : public QCloudConnection
{
public:
    QAzureConnection();
    QAzureConnection(QByteArray url, QString authentication, QByteArray storageKey);
    virtual QByteArray* get(QString name);
    virtual bool put(QCloudFile &f, QString bucket);
    virtual bool put(QCloudTable &table);
    virtual QList<QString> getBuckets();
    virtual QList<QString> getBucketContents(QString bucketName);
    virtual QNetworkReply* sendData(const QNetworkRequest req, const QByteArray &payload);
    virtual QNetworkReply* sendData(const QNetworkRequest req);
    virtual QByteArray* get(QString bucket, QString name);
    bool createContainer(QString name);
private:
    QList<QString> parseBucketListing(QNetworkReply *reply);
    QList<QString> parseBucketContentsListing(QNetworkReply *reply);
    QString dateInRFC1123();

    void initializeHeaders();
    void initializeSharedKeyLiteHeaders();

    Connection c;
    Headers head;
    virtual QNetworkRequest encode(const Request r);
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QByteArray string;
    QXmlStreamReader *reader;
};

#endif // QAZURECONNECTION_H
