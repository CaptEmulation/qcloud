#ifndef QCLOUDITEM_H
#define QCLOUDITEM_H

#include <QObject>

/*!
  \author Jarkko Laitinen
  \module QCloud

  \brief Interface for the different types of items used in the cloud. Can be a file or a table.
  */

class QCloudItem : public QObject
{
    Q_OBJECT
public:
    /**
      save function should do the following. If the file is local the changes should be changed on the local file and also
      sent to the cloud.
      */
    virtual void save() = 0;

    /**
      if the file is local as in QFile f("foo.txt") this should return true, else it will return false.
      */
    virtual bool isLocal() = 0;

    explicit QCloudItem(QObject *parent = 0);
};

#endif // QCLOUDITEM_H
