#include "qcloudbucket.h"

QCloudBucket::QCloudBucket()
{
}



QCloudBucket::QCloudBucket(QList<QString> &files) {
    this->contents = files;
}

void QCloudBucket::save() {

}

bool QCloudBucket::isLocal() {
    return true;
}

QList<QString> QCloudBucket::getBucketContents() {
    return QList<QString>();
}
