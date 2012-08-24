#ifndef HMACSHA_H
#define HMACSHA_H

#include <QObject>
#include <QCryptographicHash>
#include <QDebug>

class HmacSHA : public QObject
{
    Q_OBJECT
public:
    enum HmacSHAType { HmacSHA1, HmacSHA256 };
    explicit HmacSHA(QObject *parent = 0);
    static QByteArray hash(HmacSHAType t, QByteArray stringToSign, QByteArray secretKey);

private:
    bool testHmac(HmacSHAType t);
};

#endif // HMACSHA_H
