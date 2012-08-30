#ifndef QCLOUDFILE_H
#define QCLOUDFILE_H

#include "qclouditem.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
/**
  Author: Jarkko Laitinen
  Version: 0.1

  A stub for the future implementation of QCloudFile. Implements the QCloudItem interface.
  */

class QCloudFile : public QCloudItem
{
    Q_OBJECT
public:
    /**
      When creating QCloudFile using the constructor that takes QFile as a parameter it reads the contents of the
      file to a byte array inside the QCloudFile
      */
    QCloudFile(QFile &f);
    QCloudFile(QString fileName);

    void save();
    virtual bool isLocal();
    QString getName();
    void setName(QString name);
    qint64 getSize();
    QByteArray getContents();

private:
    QFile *file;
    QByteArray contents;
    QString name;
    bool local;
    qint64 size;

};

#endif // QCLOUDFILE_H
