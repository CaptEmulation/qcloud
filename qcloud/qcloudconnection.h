#ifndef QCLOUDCONNECTION_H
#define QCLOUDCONNECTION_H

#include <QObject>
#include <QHash>
#include <QNetworkReply>
#include "hmacsha.h"

#include "qcloudbucket.h"
#include "qcloudfile.h"
#include "qcloudtable.h"

class QCloudConnection : public QObject
{
    Q_OBJECT
public:
    /**
      Request struct contains the headers of the wanted request. The
      request is made with this in conjunction with the Headers struct
      */
    struct Request {
        QHash<QString, QString> headers;
    };

    /**
      Every new QCloudConnection contains the information of the connection
      (userid, url and secretKey)
    */
    struct Connection {
        QHash<QString, QByteArray> info;
    };

    /**
      These headers should be initialized when the connection is created.
      They should contain every one of the required or optional headers as
      this is used to create the stringToSign in the encode() function. Mostly
      used by Azure as the amount of required headers is so much greater
      (12 + canonilized headers) when compared to AMZ and their 4.
      */
    struct Headers {
        QList< QPair<QString, QString> > requiredHeaders;
    };

    QCloudConnection();
    virtual QByteArray* get(QString bucket, QString fileName) = 0;
    virtual bool put(QCloudFile &file, QString bucket) = 0;
    virtual bool put(QCloudTable &table) = 0;
    virtual QList<QString> getBuckets() = 0;
    virtual QList<QString> getBucketContents(QString bucketName) = 0;
    virtual QNetworkReply* sendData(const QNetworkRequest req) = 0;
    virtual QNetworkReply* sendData(const QNetworkRequest req, const QByteArray &payload) = 0;
private:
    virtual QNetworkRequest encode(const Request r) = 0;
};

#endif // QCLOUDCONNECTION_H
