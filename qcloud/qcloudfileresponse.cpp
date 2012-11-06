#include "qcloudfileresponse.h"

/*!
  \class QCloudFileResponse
  \brief Response class for file objects. Connects reply to a private slot, where, after
  finished has been emitted, creates a new CloudFile to the right location.

  If using QCFR the developer needs to set the cloudDir using setCloudDir and fileName
  using setFileName, as they are needed to create the file. Unless they are set filename
  of the file will be tempfile.
  */
QCloudFileResponse::QCloudFileResponse(QNetworkReply *reply) {
    this->own = reply;
    connect(reply, SIGNAL(finished()), SLOT(replyFinished()));
    this->bucket = "";
    this->filename = "";
}

/*!

  */
void QCloudFileResponse::setCloudDir(QString name) {
    this->bucket = name;
}

void QCloudFileResponse::setFileName(QString name) {
    this->filename= name;
}

void QCloudFileResponse::replyFinished() {
    if (this->own->error() == 0) {
        unparsed = this->own->readAll();
        this->file = new QCloudFile(unparsed, this->filename, this->bucket);
        this->errorno = 0;
        emit finished();
    }
    else {
        this->errorno = this->own->error();
        this->unparsed = this->own->readAll();
        this->errorMsg = this->own->errorString();
        emit failed();
    }
}

QCloudFile* QCloudFileResponse::getResponse() {
    return this->file;
}

QByteArray QCloudFileResponse::getUnparsed() {
    return this->unparsed;
}

int QCloudFileResponse::error() {
    return this->errorno;
}
