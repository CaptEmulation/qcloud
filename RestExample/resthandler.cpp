#include "resthandler.h"
#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QLocale>
#include <QUrl>

/**
  At the moment resthandler just encodes everyting in to urls
  Todo:
    Finding out if it is possible to open a connection with QNetworkAccessManager and then throwing evering in there
    Generalizing this so that it works on everything.
	
    Example:
        One could save settings in to cloud and when program starts resthandler gets them from cloud.
    
    ATM WILL NOT WORK without mine secretkey and authkey.
  */


const QString DEFAULT_HOST = "s3.amazon.com";
const QByteArray secretKey("Your secretKey");
const QString authKey("Your authKey");

RestHandler::RestHandler()
{
}
/**
  makePutRequest(QFile *file), still pretty explanatory method

  */
QByteArray RestHandler::makePutRequest(QFile *file) {
    qDebug() << "making request to put";

    QByteArray stringToSign;
    stringToSign += "PUT\n";
    stringToSign += "\n";
    stringToSign += "\n";

    QString time(QString::number((QDateTime::currentMSecsSinceEpoch()/1000)+100));
    stringToSign += time+ "\n";
    stringToSign += "/kikkare/text.txt";

    QString signature = createSignature(stringToSign);

    QByteArray url = "http://kikkare.s3-eu-west-1.amazonaws.com/text.txt";
    url.append("?AWSAccessKeyId=");
    url.append(authKey);
    url.append("&Signature=");
    url.append(signature);
    url.append("&Expires=");
    url.append(time);
    url.append("&Content-Length:");
    url.append(file->size());
    return url.data();
}

/**
  makeGetUrl(), the method name is pretty selfexplanatory.
  */
QByteArray RestHandler::makeGetUrl(){

    QByteArray stringToSign;
    stringToSign += "GET\n";
    stringToSign += "\n";
    stringToSign += "\n";

    QString time(QString::number((QDateTime::currentMSecsSinceEpoch()/1000)+100));

    stringToSign += time + "\n";
    stringToSign += "/kikkare/text.txt";

    QString signature = createSignature(stringToSign);

    QByteArray url = "http://kikkare.s3-eu-west-1.amazonaws.com/text.txt" ;
            url.append("?AWSAccessKeyId=");
            url.append(authKey);
            url.append("&Signature=");
            url.append(signature);
            url.append("&Expires=");
            url.append(time);
    return url.data();
}

/**
  A helper method to create the signature. Did not want to send the secretKey always in every
  method call.
  */
QString RestHandler::createSignature(QByteArray stringToSign){
    QString hash = hmacSha1(stringToSign, secretKey);
    qDebug() << "replacing the + and / from the hash";
    hash.replace("/","%2F");
    hash.replace("+","%2B");
    return hash;
}
/**
  * hmac-Sha1 encryption algorithm from http://qt-project.org/wiki/HMAC-SHA1
  * encrypts the signature with the secretKey.
  */
QString RestHandler::hmacSha1(QByteArray stringToSign, QByteArray secretKey) {

    qDebug() << "Calculating the hmacSha1 from the packet";
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
    if (secretKey.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        secretKey = QCryptographicHash::hash(secretKey, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
    // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for (int i = 0; i < secretKey.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ secretKey.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ secretKey.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }

    // result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(stringToSign);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
    return hashed.toBase64();
}
