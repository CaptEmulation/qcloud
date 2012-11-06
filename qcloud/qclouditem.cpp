#include "qclouditem.h"

/*!
  \class QCloudItem
  \brief Interface for the different types of items used in the cloud. Can be a file or a table.

  Some description here
  */

/*!
 Constructor
 */
QCloudItem::QCloudItem(QObject *parent) :
    QObject(parent)
{
}


/*!
  \fn QCloudItem::isLocal()
  \brief The only method to be implemented. If the file has local content this should be set to true else false

  \example QCloudFile* file = new QCloudFile(QString("cloudFile");
           file->isLocal() = false;
           QCloudFile* f2 = new QCloudFile(QByteArray("sdfasdf"), QString("localFile");
           f2->isLocal() = true;
  */
