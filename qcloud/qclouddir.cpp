#include "qclouddir.h"


/*!
  \class QCloudDir
  \brief Cloud directory

  Implementation of the QCloudItem interface. QCloudDir will be the logical match to QDir. QCloudDir will know its properties
  and the user can ask for files located inside it.

  */

/*!
  \fn QCloudDir::QCloudDir(QDir &dir)
  \brief creates new clouddir from local directory \a dir
  */
QCloudDir::QCloudDir(QDir &dir){
    contents = QList<QCloudFile *>();
    createFromALocalDirectory(dir);
}

/*!
   \brief Creates a new QCloudDir from QString \a name, also creates the local directory if it does not exist.
 */
QCloudDir::QCloudDir(QString name) {
    QDir d(name);
    if (!d.exists()) {
        d.mkpath(".");
        this->path = name;
    } else {
        createFromALocalDirectory(d);
    }
}

/*!
  \brief creates a new QCloudDir from a QDir,
  */
void QCloudDir::createFromALocalDirectory(QDir &d) {
    this->path = d.dirName();
    d.setFilter(QDir::Files);
    QFileInfoList list = d.entryInfoList();

    for (int i = 0; i < list.size(); i++) {
        QFile f(path +"/"+ list.at(i).fileName());
        contents.append(new QCloudFile(f));
    }
}
/*!
  \brief Returns a pointer to the local list of files.
  */
QList<QCloudFile *>* QCloudDir::getContents() {
    return &this->contents;
}

/*!
  \brief Returns the path used by this dir.
  */
QString QCloudDir::getPath() {
    return this->path;
}

/*!
  \brief Creates a new QCloudDir using \a files as contents and \a path as the path.

  Creates a new local directory if it does not exist.
  */
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

/*!
  \brief Adds file \a file to the current qclouddir
  */
void QCloudDir::add(QCloudFile *file) {
    this->contents.append(file);

}

/*!
  \brief Returns the file at \a at
  */
QCloudFile* QCloudDir::get(int at) {
    return contents.at(at);
}

/*!
  \brief Returns true if all of the files on the contents are local else false

  If the clouddir does not contain anything then returns true
  */
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

/*!
  \brief Returns the filenames of contained files. If the folder is empty, returns a empty QList
  */
QList<QString> QCloudDir::getCloudDirContentsAsString() {
    if (this->contents.size() == 0) {
        return QList<QString>();
    }
    QList<QString> fileNames;
    for (int i = 0; i < this->contents.size(); i++) {
        fileNames.append(this->contents.at(i)->getName());
    }
    return fileNames;
}

/*!
  \brief returns the count of files in this folder
  \return contents.size
  */
int QCloudDir::size() {
    return this->contents.size();
}
