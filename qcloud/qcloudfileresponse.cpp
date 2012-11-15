#include "qcloudfileresponse.h"

/*!
  \class QCloudFileResponse
  \brief Response class for file objects. Connects reply to a private slot, where, after
  finished has been emitted, creates a new CloudFile to the right location.

  If using QCFR the developer needs to set the cloudDir using setCloudDir and fileName
  using setFileName, as they are needed to create the file. Unless they are set filename
  of the file will be tempfile.

  QCFR is part of the refactoring of the QCloudAPI and it will be fully integrated in
  the next phase.
  */
QCloudFileResponse::QCloudFileResponse(QNetworkReply *reply) {
    this->own = reply;
    connect(reply, SIGNAL(finished()), SLOT(replyFinished()));
    this->bucket = "";
    this->filename = "";
}

/*!
    \brief Sets the qclouddir \a name of the file contained inside the QCFR
  */
void QCloudFileResponse::setCloudDir(QString name) {
    this->bucket = name;
}

/*!
  \brief Sets the file \a name of the file contained in the response
  */
void QCloudFileResponse::setFileName(QString name) {
    this->filename= name;
}

/*!
  \brief Private slot is connected to the QNetworkReply.

  replyFinished() checks if the transfer was success and if it was creates a new qcloudfile from the
  contents of qnetworkreply. If there were errors they are read to QCFR and the reply will be deleted.
  */
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
        emit cloudError();
    }
}

/*!
  \brief Returns pointer to the QCloudFile contained.
  */
QCloudFile* QCloudFileResponse::getResponse() {
    return this->file;
}

/*!
  \brief Returns the unparsed response from the cloud provider.
  */
QByteArray QCloudFileResponse::getUnparsed() {
    return this->unparsed;
}

/*!
  \brief Returns the error number. If there were no errors it will be 0 else
  it will be QNetworkReply::error();

  */
int QCloudFileResponse::error() {
    return this->errorno;
}

/*!
  \fn QCloudFileResponse::finished()
  \brief emitted when operation is finished
  */

/*!
  \fn QCloudFileResponse::cloudError()
  \brief emitted when operation did not finish.
  */
