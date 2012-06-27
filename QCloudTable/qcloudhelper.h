#ifndef QCLOUDHELPER_H
#define QCLOUDHELPER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QFile;
class QUrl;
class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;
class QSqlTableModel;
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
    QUrl makeFileRequestUrl(REQUEST_TYPE type, QString fileName);
    QUrl makeTableRequestUrl(REQUEST_TYPE type, QString domain);
    QUrl makeTableRequestUrl(QSqlTableModel &model);
    QUrl makeFileStructure();
    QUrl createDomain();
public:
    enum OPERATION_TYPE{FILE, TABLE};
    void put(QFile &file);
    void put(QSqlTableModel &model);
    void get(QString fileName, OPERATION_TYPE type);
    explicit QCloudHelper(QObject *parent = 0);
signals:
    void finished(QNetworkReply*);
private slots:
    void requestFinished(QNetworkReply*);
};

#endif // QCLOUDHELPER_H
