#include "qcloudfile.h"

void QCloudFile::save() {

}


void QCloudFile::setLocal() {
    this->local = true;
}

void QCloudFile::setContents(const QByteArray &contents) {
    this->contents = contents;
}

QCloudFile::QCloudFile(QString fileName) {
    this->local = false;
    this->name = fileName;
    this->file = new QFile(fileName);
}

QCloudFile::QCloudFile(QByteArray contents, QString fileName) {
    this->contents = contents;
    QFile f(fileName);
    if (f.exists()) {
        qDebug() << "file exists so overwriting";
        f.remove();
    }

    if (!f.open(QIODevice::ReadWrite | QFile::Text)) {
        qDebug() << "complications opening the file";
    }
    f.write(contents);
    f.close();

    this->file = new QFile(fileName);
}

QCloudFile::QCloudFile(QByteArray contents, QString fileName, QString bucket)
{
    this->contents = contents;
    QString filepath = bucket + "/" + fileName;
    qDebug() << filepath;
    QFile f(filepath);

    if (f.exists()) {
        qDebug() << "file exists so overwriting";
        f.remove();
    }

    if (!f.open(QIODevice::ReadWrite | QFile::Text)) {
        qDebug() << "complications opening the file";
    }
    f.write(contents);
    f.close();
    this->file = new QFile(fileName);
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

QFile* QCloudFile::getFile() {
    return this->file;
}

