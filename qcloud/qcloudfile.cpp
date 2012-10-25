#include "qcloudfile.h"

/*!
  \class QCloudFile
  \brief QCloudFile is a implementation of the QCloudItem interface.

  Some explanation
  */


/*!
  Sets the file as being local or not with the boolean  \a isLocal.
  */
void QCloudFile::setLocal(bool isLocal) {
    this->local = isLocal;
}

/*!
  Creates a new QCloudFile with the name \a name.
  */
QCloudFile::QCloudFile(QString fileName) {
    this->setLocal(false);
    this->name = fileName;
    this->file = new QFile(fileName);
}

/*!
   Creates a new QCloudFile and a local file named \a fileName. Also creates a local file containing \a contents.
  */
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
    this->setLocal(true);
    this->file = new QFile(fileName);
}

/*!
  Same as the previous constructor but uses the \a bucket as a directory for the file.
  */
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
    this->setLocal(true);
}

/*!
  When creating QCloudFile using the constructor that takes QFile as a parameter it reads the contents of the
  file to a byte array inside the QCloudFile. This is the default constructor to be used when creating cloudfiles
  from the local filesystem.
  */
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
    this->setLocal(true);
}

/*!
  \reimp
  */
bool QCloudFile::isLocal() {
    return this->local;
}

/*!
  If the file is local returns the contents of the file, else QByteArray("")
  */
QByteArray QCloudFile::getContents() {
    return this->contents;
}

/*!
  \brief Returns the filename of this QCloudFile
  */
QString QCloudFile::getName() {
    return this->name;
}

/*!
  Returns the file size of this QCF
  */
qint64 QCloudFile::getSize() {
    return this->size;
}

