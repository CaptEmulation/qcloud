#ifndef RESTHANDLER_H
#define RESTHANDLER_H


#include "qstring.h"
#include "qbytearray.h"
#include "qfile.h"
#include "qurl.h"


class RestHandler : public QObject
{

private:
    QString hmacSha1(QByteArray key, QByteArray secret);
    QString hmac_sha1(const QString key,const QString secret);
    QString createSignature(QByteArray key);
    qint64 fileSize(QFile &file);
    QString calculateSignature(QString authkey);

public:
    RestHandler();
    ~RestHandler();
    enum REQUEST_TYPE{PUT, GET};
    QUrl makeRequestUrl(REQUEST_TYPE type, QString fileName);

};

#endif // RESTHANDLER_H
