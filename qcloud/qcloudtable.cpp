#include "qcloudtable.h"

QCloudTable::QCloudTable()
{
}

QCloudTable::QCloudTable(QByteArray *array) {
    (*array).at(array->length());
}

void QCloudTable::save() {

}

bool QCloudTable::isLocal() {
    return true;
}
