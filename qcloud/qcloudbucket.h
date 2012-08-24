#ifndef QCLOUDBUCKET_H
#define QCLOUDBUCKET_H

#include "qclouditem.h"
#include "qcloudfile.h"

class QCloudBucket : public QCloudItem
{
public:
    QCloudBucket();
    QCloudBucket(QList<QString> &files);
    QList<QString> getBucketContents();
    void save();
    virtual bool isLocal();

private:
    QList<QString> contents;

};

#endif // QCLOUDBUCKET_Hcontext
