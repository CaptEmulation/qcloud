#include "qcloudfileresponse.h"


QCloudFileResponse::QCloudFileResponse(QNetworkReply *reply) {
    this->own = reply;
    connect(reply, SIGNAL(finished()), SLOT(replyFinished()));
}

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
