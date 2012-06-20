#include "qcloudhelper.h"
#include <QSettings>
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
#include <QUrl>
#include <QDebug>
#include <QStringList>

const QSettings set("JYU", "CloudExample");
enum REQUEST_TYPE {PUT, GET};

QCloudHelper::QCloudHelper(QObject *parent) : QObject(parent){}


/**
  This handles the making of the request urls.

  TODO: Should the request handling and other stuff be contained here. If the CloudHelper would handle everything and just return the QNetworkReply pointer
  that the program could then use as it wanted. This way the integration of this would be easier as the needed commands would go down to 1.
  */
QUrl QCloudHelper::makeRequestUrl(REQUEST_TYPE type, QString address){
    QByteArray url;
    QByteArray stringToSign;
    QStringList tokens = address.split(QRegExp("[./]"));
    QString verb = (type == GET) ? "GET" : "PUT";
    QString time = (QString::number((QDateTime::currentMSecsSinceEpoch()/1000)+200));

    stringToSign += verb + "\n";
    stringToSign += "\n";
    stringToSign += "\n";
    stringToSign += time + "\n";

    //Ugly hack, assumes that the connected address is s3.amazonaws.com, not the region specific ones. Same goes with the latter ugly hack.
    //This should be done better via other way.
    int size = tokens.size();
    QString path = (size > 3) ? (size > 4) ? ("/" + tokens.at(0) + "/"+ tokens.at(size-2) +"." + tokens.at(size-1) ) : ("/" + tokens.at(0) + "/") : "/";

    stringToSign += path;

    QString signature = createSignature(stringToSign);

    QString uglyHack = (tokens.size() > 4) ? "http://" + address : "http://" + address + "/";

    url.append(uglyHack);
    url.append("?AWSAccessKeyId=");
    url.append(set.value("password").toString());
    url.append("&Signature=");
    url.append(signature);
    url.append("&Expires=");
    url.append(time);

    if (type == PUT) {
        int size = tokens.size();
        QFile file(tokens.at(size-2) + tokens.at(size-1));
        url.append("&Content-Length:");
        url.append(fileSize(file));
    }
    return QUrl::fromEncoded(url);
}

qint64 QCloudHelper::fileSize(QFile &file){
    if (!file.open(QIODevice::ReadWrite)){
        return 0;
    }
    qint64 size = file.size();
    file.close();
    return size;
}

/**
  Amazon requires / and + to be replaced with %2F and %2B in the signatures.
  */
QString QCloudHelper::createSignature(QByteArray key) {
    QString hashed = hmacSha1(key, set.value("hash").toByteArray());
    hashed.replace("/", "%2F");
    hashed.replace("+", "%2B");
    return hashed.toUtf8();
}

QString QCloudHelper::hmacSha1(QByteArray stringToSign, QByteArray secretKey) {
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


