#ifndef QCLOUDFILE_H
#define QCLOUDFILE_H

#include "qclouditem.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>

class QCloudFile : public QCloudItem
{
public:
    QCloudFile();
    QCloudFile(QByteArray array);
    QCloudFile(QFile &f);
    QCloudFile(QString fileName);

    void save();
    virtual bool isLocal();
    QString getName();
    void setName(QString name);
    qint64 getSize();
    QByteArray getContents();
protected:
    QFile *file;
private:
    QByteArray contents;
    QString name;
    bool local;
    qint64 size;

};

#endif // QCLOUDFILE_H
