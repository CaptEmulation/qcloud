#ifndef QCLOUDITEM_H
#define QCLOUDITEM_H

#include <QObject>

class QCloudItem : public QObject
{
    Q_OBJECT
public:
    explicit QCloudItem(QObject *parent = 0);
    virtual bool isLocal() = 0;
};

#endif // QCLOUDITEM_H
