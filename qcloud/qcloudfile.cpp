#include "qcloudfile.h"

/*!
  \class QCloudFile
  \brief QCloudFile is a implementation of the QCloudItem interface.

  Contains information if the file is local or not.
  */


/*!
  \brief Sets the file as being local or not with the boolean  \a isLocal.
  */
void QCloudFile::setLocal(bool isLocal) {
    this->local = isLocal;
}

/*!
  \brief Creates a new QCloudFile with the name \a fileName.
  */
QCloudFile::QCloudFile(QString fileName) {
    this->setLocal(false);
    this->name = fileName;
    this->file = new QFile(fileName);
}

/*!
   \brief Creates a new QCloudFile and a local file named \a fileName. Also creates a local file containing \a contents.
  */
QCloudFile::QCloudFile(QByteArray contents, QString fileName) {
    this->contents = contents;
    QString name = (fileName != "") ? fileName : "tempfile";

    QFile f(name);
    if (f.exists()) {
        f.remove();
    }

    if (!f.open(QIODevice::ReadWrite | QFile::Text)) {
    }
    f.write(contents);
    f.close();
    this->setLocal(true);
    this->file = new QFile(fileName);
}

/*!
  \brief Creates a new QCloudFile and a local file named \a fileName in the directory \a bucket.
         Also creates a local file containing \a contents.
  */
QCloudFile::QCloudFile(QByteArray contents, QString fileName, QString bucket)
{
    this->contents = contents;
    QString filepath;
    QDir foo(bucket);
    if(!foo.exists()) {
        filepath = fileName;
    } else {
        if (fileName != "" && bucket != "") {
            filepath = bucket + "/" + fileName;
        }
        else if (fileName != "")
        {
            filepath = fileName;
        }
        else
        {
            filepath = "tempfile";
        }
    }

    QFile f(filepath);

    if (f.exists()) {
         f.remove();
    }

    if (!f.open(QIODevice::ReadWrite | QFile::Text)) {

    }
    f.write(contents);
    f.close();
    this->file = new QFile(fileName);
    this->setLocal(true);
}

/*!
  \brief Creates a new QCloudFile using a QFile.

  When creating QCloudFile using the constructor that takes QFile \a f as a parameter it reads the contents of the
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
  \brief If the file is local returns the contents of the file, else QByteArray("")
  */
QByteArray QCloudFile::getContents() {
    if (this->isLocal()) {
        return this->contents;
    }
    else {
        return QByteArray("");
    }
}

/*!
  \brief Returns the filename of this QCloudFile
  */
QString QCloudFile::getName() {
    return this->name;
}

/*!
  \brief Returns the file size of this QCF
  */
qint64 QCloudFile::getSize() {
    return this->size;
}

