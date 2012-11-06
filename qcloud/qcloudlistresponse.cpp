#include "qcloudlistresponse.h"

/*!
  \class QCloudListResponse
  \brief Class used for containing QNetworkReply-object that contains a list reply.

  When creating QCloudListResponse the parameter QNetworkReply will be connected to a private
  slot and when QNR emits finished() QCLR catches it, reads the unparsed text to a variable
  and emits finished().

  QCloudListResponse contains a simple XML-parser that parses the listing and finds the
  QCloudFiles/QCloudDirs from it. User can get the list with getParsed();
  */
QCloudListResponse::QCloudListResponse(QNetworkReply *reply) {
    this->reply = reply;
    connect(reply, SIGNAL(finished()), SLOT(requestFinished()));
    this->errorMsg = "";
    this->nro = 0;
    this->parsed = false;
}


/*!
  \brief Checks if the message has been parsed already, if not, parses the message and sets the parsed
  message to a local variable.
  \returns List of QString that contains the contents of the reply
  */
QList<QString> QCloudListResponse::getParsed() {
    if (!parsed) {
        QXmlStreamReader reader;
        reader.addData(this->unparsed);
        QList<QString> list;
        while (!reader.atEnd()) {
            reader.readNextStartElement();
            if (reader.name().toString() == "Name" || reader.name().toString() == "Key"){
                list.append(reader.readElementText());
            }
        }
        this->list = list;
    }
    return this->list;
}

/*!
  \brief Contains the error number from QNetworkReply.
  */
int QCloudListResponse::error() {
    return this->nro;
}

/*!
  \brief Private slot that is connected to QNetworkReply. Reads the contents and deletes the reply.
  */
void QCloudListResponse::requestFinished() {
    if (reply->error() == 0) {
        QByteArray message = reply->readAll();
        this->unparsed = message;
        emit finished();
        reply->deleteLater();
    }
    else
    {
        this->nro = reply->error();
        this->errorMsg = reply->errorString() + " " +reply->readAll() + " " ;
        emit cloudError();
        reply->deleteLater();
    }

}

/*!
  \brief returns the unparsed message received from the cloud.
  */
QByteArray QCloudListResponse::getUnparsed() {
    return this->unparsed;
}
