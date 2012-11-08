#ifndef QCLOUDFILE_H
#define QCLOUDFILE_H

#include "qclouditem.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
class QCloudFile : public QCloudItem
{
    Q_OBJECT
public:
    QCloudFile(QFile &f);
    QCloudFile(QString fileName);
    QCloudFile(QByteArray contents, QString fileName);
    QCloudFile(QByteArray contents, QString fileName, QString bucket);

    virtual bool isLocal();

    QString getName();
    qint64 getSize();
    QByteArray getContents();
    void setLocal(bool isLocal);

private:
    QFile *file;
    QByteArray contents;
    QString name;
    bool local;
    qint64 size;

};

#endif
