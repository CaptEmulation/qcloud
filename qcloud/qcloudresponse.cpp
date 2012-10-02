#include "qcloudresponse.h"

QCloudResponse::QCloudResponse(QObject *parent) : QObject(parent)
{
    this->response = 0;
    this->errorCount = 0;
}

QCloudResponse::QCloudResponse(QByteArray &response, int &error, QCloudResponse::RESPONSETYPE t) {
    this->response = response;
    this->errorCount = error;
    this->type = t;
}

QByteArray QCloudResponse::getResponse() {
    return this->response;
}

QCloudResponse::RESPONSETYPE QCloudResponse::getResponseType() {
    return this->type;
}
