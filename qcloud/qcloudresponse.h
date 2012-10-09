#ifndef QCLOUDRESPONSE_H
#define QCLOUDRESPONSE_H

#include <QObject>
#include <QNetworkReply>
/*!
  \author Jarkko Laitinen
  \module QCloud

  \brief QCloudResponse is used when QCloudConnection is used asynchronious.
  */
class QCloudResponse : public QObject
{
    Q_OBJECT
public:
    enum RESPONSETYPE { CLOUDDIR, CLOUDDIRCONTENTS, CLOUDFILE };
    explicit QCloudResponse(QObject *parent = 0);
    QCloudResponse(QByteArray &contents, int &errorCount, RESPONSETYPE t);
    QByteArray getResponse();
    int error();
    QCloudResponse::RESPONSETYPE getResponseType();
signals:
    void finished();
private:
    QByteArray response;
    int errorCount;
    QCloudResponse::RESPONSETYPE type;
    QNetworkReply *reply;

};

#endif // QCLOUDRESPONSE_H
