#include "hmacsha.h"

/**
  Library to provide the Hash-based message authentication codes using SHA1 and SHA256 algorithms. At the moment
  both are used but in the future SHA256 will be the only one.
  */

HmacSHA::HmacSHA(QObject *parent) : QObject(parent){}

/**
  Modified version of the hmacSha1 example from http://qt-project.org/wiki/HMAC-SHA1
  */
QByteArray HmacSHA::hash(HmacSHAType t, QByteArray stringToSign, QByteArray secretKey){


    QCryptographicHash::Algorithm alg = (t == HmacSHA1) ? QCryptographicHash::Sha1 : QCryptographicHash::Sha256;

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

bool HmacSHA::testHmac(HmacSHAType t) {
    switch (t) {
        case HmacSHA256: {
            QByteArray key2 = QByteArray::fromHex("0102030405060708090a0b0c0d0e0f10111213141516171819");
            QByteArray data2 = QByteArray::fromHex("cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd");
            QByteArray shouldB2 = QByteArray::fromHex("82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b");

            QByteArray hashed = HmacSHA::hash(HmacSHA::HmacSHA256, data2, key2);
            QByteArray asdf = shouldB2.toBase64();

            if (hashed == asdf) {
                return true;
            } else return false;
        }
    case HmacSHA1: {
            return true;
        }
    }
}
