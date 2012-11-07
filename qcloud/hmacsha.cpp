#include "hmacsha.h"

/*!
  \class HmacSHA
  \brief Does HMAC hashes with SHA1 and SHA256 encryptions.

  HmacSHA class provides the hash function that is needed to make hases of the stringToSign arrays. These
  are used to verify the senders identity. HmacSHA uses the \l{QCryptographicHash} from Qt5 so Qt5 is required
  or a custom build of Qt.
  */

/*!
  \enum HmacSHA::HmacSHAType
  Enumeration is used to identify the wanted type of hash, supports SHA1 and SHA256
  */

/*!
  Library to provide the Hash-based message authentication codes using SHA1 and SHA256 algorithms. At the moment
  both are used but in the future SHA256 will be the only one.
  */
HmacSHA::HmacSHA(QObject *parent) : QObject(parent){}

/*!
  Modified version of the hmacSha1 example from http://qt-project.org/wiki/HMAC-SHA1

  Takes \a stringToSign that is constructed from the request, \a secretKey that is the users
  own credential to a cloud service provider, and \a t that is the type of used algorithm (sha1 or sha256)

  returns a byte array containing the hash signature.
  */
QByteArray HmacSHA::hash(HmacSHAType t, QByteArray stringToSign, QByteArray secretKey){

    QCryptographicHash::Algorithm alg = (t == SHA1) ? QCryptographicHash::Sha1 : QCryptographicHash::Sha256;

    int blockSize = 64; // HMAC-SHA-1 & SHA-256 Blocksize

    //Hmac, as defined inf RFC 2104 requires a certain length of key (blockSize) and these next two if clauses make sure that it is like so.
    if (secretKey.length() > blockSize) {
        secretKey = QCryptographicHash::hash(secretKey, alg);
    }

    //If the length is too short, the message is padded with 0x00 as stated in the spec
    if (secretKey.length() < blockSize) {
        int padding = blockSize - secretKey.length();
        secretKey.append(QByteArray(padding, char(0x00)));
    }
    QByteArray innerPadding(blockSize, char(0x36));
    QByteArray outerPadding(blockSize, char(0x5c));

    for (int i = 0; i < secretKey.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ secretKey.at(i);
        outerPadding[i] = outerPadding[i] ^ secretKey.at(i);
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(stringToSign);
    total.append(QCryptographicHash::hash(part, alg));
    QByteArray hashed = QCryptographicHash::hash(total, alg);


    QByteArray replaced = hashed.toBase64();
    return replaced;
}
