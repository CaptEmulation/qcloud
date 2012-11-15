#ifndef QCLOUDDIR_H
#define QCLOUDDIR_H

#include "qclouditem.h"
#include "qcloudfile.h"
#include "qcloudconnection.h"
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

class QCloudDir : public QCloudItem
{
    Q_OBJECT
public:

    QCloudDir(QDir &dir);
    QCloudDir(const QList<QString> &files, QString path);
    QCloudDir(QString name);

    virtual bool isLocal();
    QCloudFile* get(int at);

    QList<QString> getCloudDirContentsAsString();
    QList<QCloudFile *>* getContents();
    void add(QCloudFile* file);

    QString getPath();
    int size();

private:
    QList<QCloudFile *> contents;
    QString path;
    void createFromALocalDirectory(QDir &d);
};

#endif
