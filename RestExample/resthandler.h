#ifndef RESTHANDLER_H
#define RESTHANDLER_H


#include "qstring.h"
#include "qbytearray.h"
#include "qfile.h"
#include "qurl.h"


class RestHandler : public QObject
{

private:
    QString user;
    QString bucket;
    QString authkey;
    QString hmacSha1(QByteArray key, QByteArray secret);
    QString hmac_sha1(const QString key,const QString secret);
    QString createSignature(QByteArray key);

public:
    RestHandler();
    QByteArray makePutRequest(QFile *file);
    QString calculateSignature(QString authkey);
    QByteArray makeGetUrl();

};

#endif // RESTHANDLER_H
