#ifndef QCLOUDRESPONSE_H
#define QCLOUDRESPONSE_H

#include <QObject>
#include <QNetworkReply>

class QCloudResponse : public QObject
{
    Q_OBJECT
public:
    QCloudResponse(QNetworkReply *reply);
    QByteArray getResponse();
    int error();
    QByteArray getError();

signals:
    void finished();
    void cloudError();

private slots:
    void requestFinished();

private:
    QByteArray response;
    int errorCount;
    QByteArray errorMsg;
    QNetworkReply *reply;
};

#endif
