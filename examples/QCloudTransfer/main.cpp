#include <QtCore/QCoreApplication>
#include "qamazonconnection.h"
#include <iostream>

QList<QString> readAuthFromFile() {
    QList<QString> auth;
    QFile f("auth.txt");
    if (!f.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "could not open file auth.txt, you sure it exists?";
    }
    QByteArray line;
    while (f.canReadLine()) {
        line = f.readLine();
        line.replace("\n", "");
        auth.append(line);
    }
    f.close(); 

    return auth;
}

void printList(QList<QString> list) {
    foreach(QString s, list) {
        qDebug() << s;
    }
}

int getFile() {
    qDebug() << "Doing get-operation";
    qDebug() << "Reading authentication from file auth.txt";
    QList<QString> auth = readAuthFromFile();
    QAmazonConnection conn(auth.at(0).toUtf8(), auth.at(1).toUtf8(), auth.at(2).toUtf8());
    qDebug() << "Connection initialized";
    QList<QString> buckets = conn.getCloudDir();
    qDebug() << "Your account contained the following buckets:";
    printList(buckets);
    qDebug() << "Which bucket would you like to inspect";
    std::string s;
    std::cin>>s;
    QString bucket(s.c_str());
    qDebug() << "Getting contents of " + bucket;
    QList<QString> files = conn.getCloudDirContents(bucket);
    qDebug() << "The bucket contained following files :";
    printList(files);
    qDebug() << "What would you like to get?";
    std::string file;
    std::cin>>file;
    QString qfile (file.c_str());
    qDebug() << "Downloading the file " + qfile;
    QCloudFile* down = conn.get(bucket, qfile);
    qDebug() << "File downloaded, thx";
    return 0;
}

int putFile(QString name) {
    qDebug() << name;
    qDebug() << "Doing put-operation";
    qDebug() << "Reading authentication from file auth.txt";
    QList<QString> auth = readAuthFromFile();
    QAmazonConnection conn(auth.at(0).toUtf8(), auth.at(1).toUtf8(), auth.at(2).toUtf8());
    qDebug() << "Connection initialized";
    QList<QString> buckets = conn.getCloudDir();
    qDebug() << "Your account contained the following buckets:";
    printList(buckets);
    qDebug() << "Please specify in which bucket would you like to place the file";
    std::string bucket;
    std::cin>>bucket;
    QString qbucket(bucket.c_str());
    if (!buckets.contains(qbucket)) {
        qDebug() << "The bucket did not exists. exitting";
        return -1;
    }
    qDebug() << QString("Putting file %1 to bucket %2").arg(name).arg(qbucket);
    QFile f(name);
    if (!f.exists()) {
        qDebug() << "File did not exist, exitting";
        return -1;
    }
    QCloudFile cf(f);
    if (conn.put(cf, qbucket)) {
        qDebug() << "File uploaded, exitting";
        return 0;
    }

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc < 2) {
        qDebug() << "You did not specify operation, please use qcloudtransefer (put or get)";
        return 0;
    }

    if (strcmp(argv[1], "put") == 0) {
        putFile(QString(argv[2]));
    } else if (argv[1]){
        getFile();
    } else {
       qDebug() << "Please specify operation, put or get" ;
    }
        //return a.exec();
}
