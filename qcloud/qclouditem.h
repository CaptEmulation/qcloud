#ifndef QCLOUDITEM_H
#define QCLOUDITEM_H

#include <QObject>

class QCloudItem : public QObject
{
    Q_OBJECT
public:
    virtual bool isLocal() = 0;

protected:
    explicit QCloudItem(QObject *parent = 0);
};

#endif // QCLOUDITEM_H
