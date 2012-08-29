#ifndef QCLOUDTABLE_H
#define QCLOUDTABLE_H

#include "qclouditem.h"

/**
  Author: Jarkko Laitinen
  Version: 0.1

  QCloudTable is nofunc at the moment. There is still some decisions to be made before this class is implemented.
  */

class QCloudTable : public QCloudItem
{
    Q_OBJECT
public:
    QCloudTable();
    QCloudTable(QByteArray* array);
    void save();
    virtual bool isLocal();
};

#endif // QCLOUDTABLE_H
