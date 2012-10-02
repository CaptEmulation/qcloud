#ifndef QCLOUDRESPONSE_H
#define QCLOUDRESPONSE_H

#include <QObject>

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
    void finished(QCloudResponse*);
private:

    QByteArray response;
    int errorCount;
    QCloudResponse::RESPONSETYPE type;
};

#endif // QCLOUDRESPONSE_H
