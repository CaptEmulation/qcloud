#include "qclouditem.h"

/*!
  \class QCloudItem
  \brief Interface for the different types of items used in the cloud. Can be a file or a table.

  QCloudItem is a wrapper class for QDir and QFile that contains the file and information about the location.
  The only implementable function is isLocal() that returns the value of locality
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
