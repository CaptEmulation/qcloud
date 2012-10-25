#include "qcloudlistresponse.h"

/*!
  \class QCloudListResponse
  \brief foobar

  foobar
  */

QCloudListResponse::QCloudListResponse(QObject *parent) :
    QObject(parent)
{
}

QCloudListResponse::QCloudListResponse(QNetworkReply *reply) {
    this->reply = reply;
    connect(reply, SIGNAL(finished()), SLOT(requestFinished()));
    this->errorMsg = "";
    this->nro = 0;
}


QList<QString> QCloudListResponse::getParsed() {
    return this->list;
}

int QCloudListResponse::error() {
    return this->nro;
}

void QCloudListResponse::requestFinished() {
    qDebug() << "from requestFinished()";
    if (reply->error() == 0) {
        qDebug() << "error == 0;";
        QByteArray message = reply->readAll();
        this->unparsed = message;
        QXmlStreamReader reader;
        reader.addData(message);
        QList<QString> list;
        while (!reader.atEnd()) {
            reader.readNextStartElement();
            if (reader.name().toString() == "Name" || reader.name().toString() == "Key"){
                list.append(reader.readElementText());
            }
        }
        this->list = list;
        emit finished();
    }
    else
    {
        qDebug() << "errrorororoiror";
        this->nro = reply->error();
        this->errorMsg = reply->errorString() + " " +reply->readAll() + " " ;
        emit someError();
    }

}

QByteArray QCloudListResponse::getUnparsed() {
    return this->unparsed;
}
