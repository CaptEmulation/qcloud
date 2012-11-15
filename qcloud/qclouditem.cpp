#include "qclouditem.h"

/*!
  \class QCloudItem
  \brief Interface for the different types of items used in the cloud. Can be a file or a table.

  QCloudItem is a wrapper class for QDir and QFile that contains the file and information about the location.
  The only implementable function is isLocal() that returns the value of locality
  */

/*!
 Constructor \a parent
 */
QCloudItem::QCloudItem(QObject *parent) :
    QObject(parent)
{
}


/*!
  \fn QCloudItem::isLocal()
  \brief The only method to be implemented.
  \return If the file has local content this should be set to true else false
  */
