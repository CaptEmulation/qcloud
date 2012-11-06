#include "qcloudresponse.h"

/*!
  \class QCloudResponse

  \brief QCloudResponse is the container for a boolean value received from a cloud
  service. It contains the information was the operation success or failure.
  */
QCloudResponse::QCloudResponse(QNetworkReply *reply) {
    this->reply = reply;
    connect(reply, SIGNAL(finished()), this, SLOT(requestFinished()));
    this->errorCount = 0;
    this->response = "";
    this->errorMsg = "";
}

/*!
  \brief Returns the response from cloud. Response has not been parsed
  */
QByteArray QCloudResponse::getResponse() {
    return this->response;
}

/*!
  \brief Private slot that checks if the request was success or failure.
  */
void QCloudResponse::requestFinished() {
    if (this->reply->error() == 0) {
        this->errorCount = 0;
        response = reply->readAll();
        reply->deleteLater();
        emit finished();
    } else {
        this->errorCount = this->reply->error();
        response = reply->readAll();
        errorMsg = reply->errorString().toUtf8();
        reply->deleteLater();
        emit cloudError();
    }
}

/*!
  \brief Returns the error message.
  */
QByteArray QCloudResponse::getError() {
    if (this->error() == 0) {
        return QByteArray("");
    }
    else
    {
        return this->errorMsg;
    }
}
