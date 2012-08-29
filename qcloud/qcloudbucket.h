#ifndef QCLOUDBUCKET_H
#define QCLOUDBUCKET_H

#include "qclouditem.h"
#include "qcloudfile.h"

/**
  Author: Jarkko Laitinen
  Version: 0.1

  Implementation of the QCloudItem interface. QCloudBucket will be the logical match to QDir. QCloudBucket will know its properties
  and the user can ask for files located inside it.
  */
class QCloudBucket : public QCloudItem
{
    Q_OBJECT
public:
    QCloudBucket();

    /**
      creates a new QCloudBucket from the list of files. This will be changed to QDir as this is used in conjunction with local files.
      */
    QCloudBucket(QList<QString> &files);

    /**
      returns the contents of this bucket. at the moment nofunc.
    */
    QList<QString> getBucketContents();

    /**
      When calling save() the changes to files inside this bucket are sent to the cloud, using the QCloudConnections.
      */
    void save();

    /**
      if the file is local = true else false
      */
    virtual bool isLocal();

private:
    QList<QString> contents;

};

#endif // QCLOUDBUCKET_Hcontext
