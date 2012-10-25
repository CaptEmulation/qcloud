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
  if the file is local as in QFile f("foo.txt") this should return true, else it will return false.
  */
