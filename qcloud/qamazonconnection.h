#ifndef QAMAZONCONNECTION_H
#define QAMAZONCONNECTION_H

#include "qcloudconnection.h"
#include <QString>
#include <QHash>
#include <QXmlStreamReader>
#include "qcloudfile.h"

class QAmazonConnection : public QCloudConnection
{
public:

    QAmazonConnection();
    QAmazonConnection(QByteArray host, QByteArray user, QByteArray password, QByteArray secret);
    virtual QByteArray* get(QString bucket, QString fileName);
    virtual bool put(QCloudFile &f, QString bucket);
    virtual bool put(QCloudTable &table);
    virtual QList<QString> getBuckets();
    virtual QNetworkReply* sendData(QNetworkRequest req);
    virtual QNetworkReply* sendData(const QNetworkRequest req, const QByteArray &payload);
    virtual QList<QString> getBucketContents(QString bucketName);

private:
    virtual QNetworkRequest encode(const Request r);

    Connection c;
    Request r;
    Headers h;

    void replaceUnallowed(QByteArray *array);
    void prepare(QNetworkReply *);
    QList<QString> parseBucketContentListing(QByteArray *array);
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QByteArray secret;
    QHash<QString, QString> headers;
    QList<QString> parseBucketListings(QByteArray* message);
    QXmlStreamReader *reader;
};

#endif // QAMAZONCONNECTION_H
