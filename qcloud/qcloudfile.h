#ifndef QCLOUDFILE_H
#define QCLOUDFILE_H

#include "qclouditem.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
/**
  \author Jarkko Laitinen
  \module QCloud

  \brief QCloudFile is a implementation of the QCloudItem interface.
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
    QCloudFile(QByteArray contents, QString fileName);
    QCloudFile(QByteArray contents, QString fileName, QString bucket);
    void save();
    virtual bool isLocal();
    QString getName();
    void setName(QString name);
    qint64 getSize();
    QByteArray getContents();
    QFile* getFile();
    void setLocal();
    void setContents(const QByteArray &contents);
private:
    QFile *file;
    QByteArray contents;
    QString name;
    bool local;
    qint64 size;

};

#endif // QCLOUDFILE_H
