#ifndef QCLOUDLISTRESPONSE_H
#define QCLOUDLISTRESPONSE_H

#include <QObject>
#include <QNetworkReply>
#include <QList>
#include <QXmlStreamReader>
#include "qclouddir.h"
#include <QDebug>

class QCloudListResponse : public QObject
{
    Q_OBJECT
public:
    explicit QCloudListResponse(QObject *parent = 0);
    QList<QString> getParsed();
    QCloudListResponse(QNetworkReply *reply);
    int error();
    QByteArray getUnparsed();
private:
    QNetworkReply *reply;
    QList<QString> list;
    QString errorMsg;
    int nro;
    QByteArray unparsed;
signals:
    void finished();
    void someError();
private slots:
    void requestFinished();
};

#endif // QCLOUDLISTRESPONSE_H
