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
    /*!
      When creating QCloudFile using the constructor that takes QFile as a parameter it reads the contents of the
      file to a byte array inside the QCloudFile. This is the default constructor to be used when creating cloudfiles
      from the local filesystem.
      */
    QCloudFile(QFile &f);

    /*!
      Creates a new QCloudFile with the name \a name.
      */
    QCloudFile(QString fileName);

    /*!
       Creates a new QCloudFile and a local file named \a fileName. Also creates a local file containing \a contents.
      */
    QCloudFile(QByteArray contents, QString fileName);

    /*!
      Same as the previous constructor but uses the \a bucket as a directory for the file.
      */
    QCloudFile(QByteArray contents, QString fileName, QString bucket);

    /*!
      \reimp
      */
    virtual bool isLocal();

    /*!
      \fn QCloudFile::getName()

      \brief Returns the filename of this QCloudFile
      */
    QString getName();

    /*!
      Returns the file size of this QCF
      */
    qint64 getSize();

    /*!
      If the file is local returns the contents of the file, else QByteArray("")
      */
    QByteArray getContents();

    /*!
      Sets the file as being local or not with the boolean  \a isLocal.
      */
    void setLocal(bool isLocal);

private:
    QFile *file;
    QByteArray contents;
    QString name;
    bool local;
    qint64 size;

};

#endif // QCLOUDFILE_H
