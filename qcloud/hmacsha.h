#ifndef HMACSHA_H
#define HMACSHA_H

#include <QObject>
#include <QCryptographicHash>

class HmacSHA : public QObject
{
    Q_OBJECT
public:
    enum HmacSHAType {
        SHA1,
        SHA256
    };

    explicit HmacSHA(QObject *parent = 0);

    static QByteArray hash(HmacSHAType t, QByteArray stringToSign, QByteArray secretKey);
};

#endif // HMACSHA_H
