#ifndef QCLOUDFILERESPONSE_H
#define QCLOUDFILERESPONSE_H

#include <QNetworkReply>
#include <QDebug>
#include "qcloudfile.h"
#include <QObject>

class QCloudFileResponse : public QObject
{
    Q_OBJECT
public:
    QCloudFileResponse(QNetworkReply* reply);
    QCloudFile* getResponse();
    QByteArray getUnparsed();
    void setFileName(QString name);
    void setCloudDir(QString name);
    int error();

signals:
    void finished();
    void failed();

private slots:
    void replyFinished();

private:
    QNetworkReply *own;
    QCloudFile* file;
    QByteArray unparsed;
    QString filename;
    QString bucket;
    QString errorMsg;
    int errorno;
};

#endif
