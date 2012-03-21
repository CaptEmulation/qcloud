#include "resthandler.h"
#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QUrl>
#include <QSettings>

/**
  At the moment resthandler just encodes everyting in to urls
  Todo:
    Finding out if it is possible to open a connection with QNetworkAccessManager and then throwing evering in there
    Generalizing this so that it works on everything.

    Example:
        One could save settings in to cloud and when program starts resthandler gets them from cloud.

    The main window(window.cpp) call this class to make the REST requests that are needed to access the Amazon S3. The call goes
    to RestHandler::makeRequestUrl(), params being the type of request and what the request wants. With these informations the
    request and the authorization signature is made.

  Questions:
    should the settings be encrypted?
    if a developer ships a software with cloud capabilities can the client mine the settings from the app?
    this kind of action is really not wanted because if the client finds the secretKey and authKey they can access
    the cloud with every right.
  */

const QSettings settings("RestExample", "RestHandler");
enum REQUEST_TYPE {PUT, GET};

RestHandler::RestHandler(){}

QUrl RestHandler::makeRequestUrl(REQUEST_TYPE type, QString fileName)
{
    QString bucket = "kikkare";
    QByteArray url;
    QByteArray stringToSign;
    QString verb = (type == GET) ? "GET" : "PUT";
    QString time(QString::number((QDateTime::currentMSecsSinceEpoch()/1000)+200));

    stringToSign += verb + "\n";
    stringToSign += "\n";
    stringToSign += "\n";
    stringToSign += time + "\n";
    stringToSign += "/" + bucket + "/" + fileName; // this needs to be rethinked.
    QString signature = createSignature(stringToSign);

    url.append("http://" + bucket + ".s3-eu-west-1.amazonaws.com/" + fileName);
    url.append("?AWSAccessKeyId=");
    url.append(settings.value("authKey").toString());
    url.append("&Signature=");
    url.append(signature);
    url.append("&Expires=");
    url.append(time);
    if (type == PUT) {
        QFile file(fileName);
        url.append("&Content-Length:");
        url.append(fileSize(file));
    }
    return QUrl::fromEncoded(url);

}

qint64 RestHandler::fileSize(QFile &file)
{
    if (!file.open(QIODevice::ReadWrite))
        return 0;
    return file.size();
}

/**
  A helper method to create the signature. Did not want to send the secretKey always in every
  method call.
  */
QString RestHandler::createSignature(QByteArray stringToSign)
{
    QString hashed = hmacSha1(stringToSign, settings.value("secretKey").toByteArray());
    hashed.replace("/", "%2F");
    hashed.replace("+", "%2B");
    return hashed.toUtf8();
}

/**
  * hmac-Sha1 encryption algorithm from http://qt-project.org/wiki/HMAC-SHA1
  * encrypts the signature with the secretKey.
  */
QString RestHandler::hmacSha1(QByteArray stringToSign, QByteArray secretKey)
{ 
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
