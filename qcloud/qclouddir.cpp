#include "qclouddir.h"

QCloudDir::QCloudDir(QDir &dir){
    contents = QList<QCloudFile *>();
    createFromALocalDirectory(dir);
}

void QCloudDir::createFromALocalDirectory(QDir &d) {
    this->path = d.dirName();
    d.setFilter(QDir::Files);
    QFileInfoList list = d.entryInfoList();
    foreach (QFileInfo i , list) {
        QFile f(path +"/"+ i.fileName());
        contents.append(new QCloudFile(f));
    }
}

QList<QCloudFile *>* QCloudDir::getContents() {
    return &this->contents;
}

QString QCloudDir::getPath() {
    return this->path;
}

QCloudDir::QCloudDir(const QList<QString> &files, QString path) {
    contents = QList<QCloudFile *>();
    this->path = path;
    QDir d(path);
    if (d.exists()) {
        createFromALocalDirectory(d);
    } else {
        d.mkpath(".");
    }

    QList<QString> list = this->getCloudDirContentsAsString();
    for (int i = 0; i < files.size(); i++) {
        if (!list.contains(files.at(i))) {
            contents.append(new QCloudFile(files.at(i)));
        }
    }
}

void QCloudDir::add(QCloudFile *file) {
    this->contents.append(file);

}

QCloudFile* QCloudDir::get(int at) {
    return contents.at(at);
}

bool QCloudDir::isLocal() {
    int size = this->contents.size();
    if (size == 0) {
        return true;
    }
    for (int i = 0; i < size; i++) {
        if (!contents.at(i)->isLocal())
            return false;
    }
    return true;
}

void QCloudDir::setContents(QList<QCloudFile *> *contents) {
    delete &this->contents;
    this->contents = (*contents);
}

QList<QString> QCloudDir::getCloudDirContentsAsString() {
    QList<QString> fileNames;
    for (int i = 0; i < this->contents.size(); i++) {
        fileNames.append(this->contents.at(i)->getName());
    }
    return fileNames;
}

int QCloudDir::size() {
    return this->contents.size();
}
