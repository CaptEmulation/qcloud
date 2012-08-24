#include <QtCore/QCoreApplication>
#include "main.h"
#include "hmacsha.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    /*
    QAmazonConnection *connection = new QAmazonConnection("s3.amazonaws.com", "jlaitinen", "AKIAJU2NIL3TMN5IWC2Q", "QVzEBqo4Q/v6WfOskqnLQelV9fHTu9p5agGI2tNK");
    QList<QString>::iterator i;

    QList<QString> buckets = connection->getBuckets();
    if (buckets.size() > 0) {
        qDebug() << "buckets size > 0";
        for (i = buckets.begin(); i != buckets.end(); ++i) {
            qDebug() << *i;
        }
    }


    QList<QString> contents = connection->getBucketContents(buckets.at(0));

    if (contents.size() > 0) {
        for (i = contents.begin(); i != contents.end(); ++i) {
            qDebug() << *i;
        }
    }
    QByteArray *file = connection->get(buckets.at(0), contents.at(0));
    qDebug() << (*file);


    QFile f("c:/MyTemp/test.txt");
    QCloudFile *cf = new QCloudFile(f);

    qDebug() << cf->getName();
    qDebug() << connection->put(*cf, buckets.at(0));
    QCloudFile *cloud = new QCloudFile(QString("file.txt"));
    qDebug() << cloud->isLocal();
    return a.exec();
    */


    QAzureConnection *connection = new QAzureConnection(QByteArray("http://jagebage.blob.core.windows.net"), QString("SharedKey"), QByteArray::fromBase64("vv2g7JnOo1ob+mUErCPXl7u4GbuWENN5JnMHGn+Wydnlr0o2Tg54TnpQv2wm/1CeN/MXfGR63FstnuSkc8bI8g=="));

    QList<QString> buckets = connection->getBuckets();

    for (int i = 0; i < buckets.size(); i++) {
        qDebug() << buckets.at(i);
    }

    QList<QString> files = connection->getBucketContents(buckets.at(0));

    foreach (QString string , files) {
        qDebug() << string;
    }
    QFile f("/home/jagbg/Work/qcloud/file.txt");
    QCloudFile cf(f);
    qDebug() << connection->put(cf, buckets.at(0));

    return 0;
}


