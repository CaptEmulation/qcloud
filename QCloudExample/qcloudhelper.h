#ifndef QCLOUDHELPER_H
#define QCLOUDHELPER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QFile;
class QUrl;
class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;
QT_END_NAMESPACE

class QCloudHelper : public QObject
{
    Q_OBJECT

private:
    enum REQUEST_TYPE{PUT, GET};
    QString hmacSha1(QByteArray key, QByteArray secret);
    QString createSignature(QByteArray key);
    qint64 fileSize(QFile &file);
    QString calculateSignature(QString authkey);
    QString currentPath;
    QNetworkReply *reply;
    QNetworkAccessManager *manager;
    QUrl makeRequestUrl(REQUEST_TYPE type, QString fileName);
public:
    void put(QFile &file);
    void get(QString fileName);
    explicit QCloudHelper(QObject *parent = 0);
signals:
    void finished(QNetworkReply*);
private slots:
    void requestFinished(QNetworkReply*);
};

#endif // QCLOUDHELPER_H
