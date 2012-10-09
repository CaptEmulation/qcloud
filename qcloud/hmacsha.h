#ifndef HMACSHA_H
#define HMACSHA_H

#include <QObject>
#include <QCryptographicHash>
#include <QDebug>

/*!
  \author Jarkko Laitinen
  \class HMacSha
  \brief Does HMAC hashes with SHA1 and SHA256 encryptions.

  \reentrant
  HmacSHA class provides the hash function that is needed to make hases of the stringToSign arrays. These
  are used to verify the senders identity. HmacSHA uses the \l{QCryptographicHash} from Qt5 so Qt5 is required
  or a custom build of Qt.
  */
class HmacSHA : public QObject
{
    Q_OBJECT
public:

    /*!
      Enumeration is used to identify the wanted type of hash, supports SHA1 and SHA256
      */
    enum HmacSHAType { SHA1, SHA256 };

    /*!
      hash function takes three parameters, enumeration containing the type of wanted SHA-algorithm,
      stringToSign created from the message and the secretKey of the service.
      */
    static QByteArray hash(HmacSHAType t, QByteArray stringToSign, QByteArray secretKey);

    explicit HmacSHA(QObject *parent = 0);
};

#endif // HMACSHA_H
