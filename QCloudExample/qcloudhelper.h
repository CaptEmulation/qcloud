#ifndef QCLOUDHELPER_H
#define QCLOUDHELPER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QFile;
class QUrl;
class QNetworkReply;
QT_END_NAMESPACE

class QCloudHelper : public QObject
{
    Q_OBJECT

private:
    QString hmacSha1(QByteArray key, QByteArray secret);
    QString createSignature(QByteArray key);
    qint64 fileSize(QFile &file);
    QString calculateSignature(QString authkey);

public:
    QNetworkReply* get(QString fileName);
    explicit QCloudHelper(QObject *parent = 0);
    enum REQUEST_TYPE{PUT, GET};
    QUrl makeRequestUrl(REQUEST_TYPE type, QString fileName);
signals:

};

#endif // QCLOUDHELPER_H
