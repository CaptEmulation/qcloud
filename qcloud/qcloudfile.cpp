#include "qcloudfile.h"

void QCloudFile::save() {

}


QCloudFile::QCloudFile(QString fileName) {
    this->local = false;
    this->name = fileName;
    this->file = &QFile("filename");
}

QCloudFile::QCloudFile(QFile &f) {
    QFileInfo fileInfo(f);
    this->file = &f;
    this->name = fileInfo.fileName();
    this->size = f.size();

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open the file, contents set to """;
        this->contents = "";
        return;
    }
    this->contents = f.readAll();
    f.close();
}


bool QCloudFile::isLocal() {
    return local;
}

QByteArray QCloudFile::getContents() {
    return this->contents;
}

QString QCloudFile::getName() {
    return this->name;
}
qint64 QCloudFile::getSize() {
    return this->size;
}

void QCloudFile::setName(QString name) {
    this->name = name;
}
