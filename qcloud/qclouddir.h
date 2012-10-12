#ifndef QCLOUDDIR_H
#define QCLOUDDIR_H

#include "qclouditem.h"
#include "qcloudfile.h"
#include "qcloudconnection.h"
#include <QDir>

/**
  \author Jarkko Laitinen
  \module QCloud

  \brief Implementation of the QCloudItem interface. QCloudBucket will be the logical match to QDir. QCloudBucket will know its properties
  and the user can ask for files located inside it.
  */

class QCloudDir : public QCloudItem
{
    Q_OBJECT
public:

    QCloudDir(QDir &dir);
    QCloudDir(const QList<QString> &files, QString path);

    QList<QString> getCloudDirContentsAsString();
    void add(QCloudFile* file);
    QCloudFile* get(int at);

    /**
      If one of the files is not local then returns false, otherwise true.
      */
    virtual bool isLocal();
    QList<QCloudFile *>* getContents();
    QString getPath();
    void setContents(QList<QCloudFile *> *files);
private:
    QList<QCloudFile *> contents;
    QString path;
    void createFromALocalDirectory(QDir &d);


signals:
    void transferFinished();
};

#endif // QCLOUDDIR_Hcontext
