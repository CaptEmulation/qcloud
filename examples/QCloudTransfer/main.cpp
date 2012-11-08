#include <QtCore/QCoreApplication>
#include "qamazonconnection.h"
#include <iostream>

QList<QByteArray> readAuthFromFile() {
    //EI TOIMI VIELÄ, \n hämäs
    //Lisää tunnarit tunnarit.append() järjestyksessä:
    //tunnus, AWSAccessKey, secretkey

    /*
    QList<QByteArray> auth;
    QFile f("auth.txt");
    if (!f.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "could not open file auth.txt, you sure it exists?";
    }
    QByteArray line;
    while ((line = f.readLine()) != 0) {
        auth.append(line);
    }
    f.close();
    */

    QList<QByteArray> tunnarit;

    return tunnarit;
}
void printList(QList<QString> list) {
    foreach(QString s, list) {
        qDebug() << s;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Reading authentication from file auth.txt";
    QList<QByteArray> auth = readAuthFromFile();

    QAmazonConnection conn(auth.at(0), auth.at(1), auth.at(2));
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
    //return a.exec();
}
