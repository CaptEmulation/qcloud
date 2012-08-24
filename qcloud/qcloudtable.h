#ifndef QCLOUDTABLE_H
#define QCLOUDTABLE_H

#include "qclouditem.h"

class QCloudTable : public QCloudItem
{
public:
    QCloudTable();
    QCloudTable(QByteArray* array);
    void save();
    virtual bool isLocal();
};

#endif // QCLOUDTABLE_H
