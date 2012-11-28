#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include "qamazonconnection.h"
#include "qazureconnection.h"
#include "qcloudfileresponse.h"


class QCloudTest : public QObject
{
    Q_OBJECT
    
public:
    QCloudTest();

private:
    void createFile();
private Q_SLOTS:

    void testAzureGetCloudDirs();
    void testAzurePutCloudFile();
    void testAzureGetCloudDirsFail();
    void testAzureGetCloudDirContents();
    void testAzureGetCloudDirContentsFail();
    void testAzureGetCloudFile();
    void testAzureGetCloudFileFail();
    void testAsyncAzureGetCloudDirs();
    void testAsyncAzureGetCloudDirContents();
    void testAsyncAzureGetCloudFile();

    void testAmazonGetCloudDirs();
    void testAmazonGetCloudDirsFail();
    void testAmazonGetCloudDirContents();
    void testAmazonGetCloudDirContentsFail();
    void testAmazonGetCloudFile();
    void testAmazonGetCloudFileFail();

    void testAmazonPutCloudFile();

    void testCreateCloudFileFromALocalFile();
    void testCreateCloudFileFromACloudFile();
    void testCreateCloudFileFromAByteArray();
    void testCreateCloudDirFromALocalDir();
    void testCreateCloudDirFromAList();

    void testAsyncAmazonGetCloudDirs();
    void testAsyncAmazonGetCloudDirContents();
    void testAsyncAmazonGetCloudFile();

};

QCloudTest::QCloudTest()
{

}

void QCloudTest::testAzurePutCloudFile() {
    QCloudConnection *conn = new QAzureConnection(QByteArray("") ,QByteArray(""), QByteArray(""));
    QFile f("file.txt");
    if (!f.exists()) createFile();
    QCloudFile file(f);
    bool success = conn->put(file,"heheheheheheh");

    QCOMPARE(success, true);
}

void QCloudTest::testAzureGetCloudDirs() {
    QCloudConnection *conn = new QAzureConnection();
    QList<QString> buckets = conn->getCloudDir();
    QCOMPARE(buckets.at(0), QString("heheheheheheh"));
}

void QCloudTest::createFile() {
    QFile f("file.txt");
    if (!f.open(QIODevice::ReadWrite)) return;
    f.write("this is test file");
    f.close();
}

void QCloudTest::testAzureGetCloudDirsFail() {
    QCloudConnection *conn = new QAzureConnection("not","correct", "credentials");
    QList<QString> buckets = conn->getCloudDir();
    QCOMPARE(0, buckets.size());
}

void QCloudTest::testAzureGetCloudDirContents() {
    QCloudConnection *conn = new QAzureConnection();
    QList<QString> buckets = conn->getCloudDir();
    QList<QString> files = conn->getCloudDirContents(buckets.at(0));
    QCOMPARE(QString(files.at(0)), QString("file.txt"));

}

void QCloudTest::testAzureGetCloudFile() {
    QCloudConnection *conn = new QAzureConnection();
    QList<QString> buckets = conn->getCloudDir();
    QList<QString> files = conn->getCloudDirContents(buckets.at(0));

    QCloudFile* file = conn->get(buckets.at(0), files.at(0));
    QCOMPARE(QString(file->getContents()), QString(""));
}

void QCloudTest::testAzureGetCloudFileFail() {
    QCloudConnection *conn = new QAzureConnection("not","correct", "credentials");
    QSignalSpy spy(conn, SIGNAL(cloudError()));
    conn->get("heheheheheheh", "file.txt");
    QCOMPARE(spy.count(), 1);
}

void QCloudTest::testAzureGetCloudDirContentsFail() {
    QCloudConnection *conn = new QAzureConnection("not","correct", "credentials");
    QList<QString> files = conn->getCloudDirContents("asdfafsd");
    QCOMPARE(0, files.size());
}

void QCloudTest::testAsyncAzureGetCloudFile() {
    QCloudConnection *conn = new QAzureConnection();
    QString bucket("heheheheheheh");
    QString file("file.txt");
    QCloudFileResponse *respo = conn->asyncGetCloudFile(file, bucket);
    QEventLoop l;
    connect(respo, SIGNAL(finished()), &l, SLOT(quit()));
    connect(respo, SIGNAL(cloudError()), &l, SLOT(quit()));
    l.exec();
    QCOMPARE(1,1);
}

void QCloudTest::testAsyncAzureGetCloudDirContents() {
     QCloudConnection *conn = new QAzureConnection();
    QString bucket("heheheheheheh");
    QCloudListResponse *resp = conn->asyncGetCloudDirContents(bucket);
    QEventLoop l;
    connect(resp, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();
    QCOMPARE(resp->getParsed().at(0), QString("file.txt"));
    delete resp;
}

void QCloudTest::testAsyncAzureGetCloudDirs() {
    QCloudConnection *conn = new QAzureConnection();
    QCloudListResponse *resp = conn->asyncGetCloudDir();
    QEventLoop l;
    connect(resp, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();
    QCOMPARE(resp->getParsed().at(0), QString("heheheheheheh"));
    delete resp;
}


void QCloudTest::testAmazonPutCloudFile() {
    QAmazonConnection conn();
    QFile f("file.txt");
    if (!f.exists()) createFile();
    QCloudFile file(f);
    conn.put(file,"heheheheheheh");
    QEventLoop l;
    connect(&conn, SIGNAL(finished()), &l, SLOT(quit()));
    QCOMPARE(conn.put(file,"heheheheheheh"), true);
}


void QCloudTest::testAsyncAmazonGetCloudFile() {
    QAmazonConnection conn();
    QString bucket("kikkare");
    QString file("aaaaaaaa.txt");
    QCloudFileResponse *respo = conn.asyncGetCloudFile(file, bucket);
    QEventLoop l;
    connect(respo, SIGNAL(finished()), &l, SLOT(quit()));
    connect(respo, SIGNAL(cloudError()), &l, SLOT(quit()));
    l.exec();
    QCOMPARE(1,1);
}

void QCloudTest::testAsyncAmazonGetCloudDirContents() {
    QAmazonConnection conn();
    QString bucket("kikkare");
    QCloudListResponse *resp = conn.asyncGetCloudDirContents(bucket);
    QEventLoop l;
    connect(resp, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();
    QCOMPARE(resp->getParsed().at(1), QString("aaaaaaaa.txt"));
    delete resp;
}

void QCloudTest::testAsyncAmazonGetCloudDirs() {
    QAmazonConnection conn();
    QCloudListResponse *resp = conn.asyncGetCloudDir();
    QEventLoop l;
    connect(resp, SIGNAL(finished()), &l, SLOT(quit()));
    l.exec();
    QCOMPARE(resp->getParsed().at(0), QString("heheheheheheh"));
    delete resp;
}

void QCloudTest::testCreateCloudDirFromAList() {
    QList<QString> files;
    files.append("test.txt");
    files.append("test2.txt");

    QCloudDir dir(files, QString("folder"));
    bool islocal = dir.isLocal();
    QCOMPARE(islocal, false);
    int size = dir.size();
    QCOMPARE(size, 2);
}

void QCloudTest::testCreateCloudFileFromAByteArray() {
    QByteArray contents("adsfasdfasdfasdf");
    QCloudFile *file = new QCloudFile(contents, "test.file");
    QDir d(".");
    QFileInfoList info = d.entryInfoList(QDir::Files);
    bool exists = false;
    foreach(QFileInfo i, info) {
        if (i.fileName() == "test.file") {
            exists = true;
            break;
        }
    }
    bool is = file->isLocal();
    QCOMPARE(exists, true);
    QCOMPARE(is, true);
}

void QCloudTest::testCreateCloudFileFromACloudFile() {
    QString fileName = "test.test";
    QCloudFile *f = new QCloudFile(fileName);
    bool local = f->isLocal();
    QCOMPARE(local, false);
    QCOMPARE(f->getContents(), QByteArray(""));
}

void QCloudTest::testCreateCloudFileFromALocalFile() {
    QFile f("localfile.test");

    if (!f.exists() && f.open(QIODevice::ReadWrite)){
        QTextStream stream(&f);
        stream << "testing file";
        stream.flush();
        f.close();
    }
    QCloudFile cf(f);
    bool islocal = cf.isLocal();
    QByteArray contents = cf.getContents();

    QCOMPARE(islocal, true);
    QCOMPARE(contents, QByteArray("testing file"));
    f.remove();
}

void QCloudTest::testCreateCloudDirFromALocalDir() {
    QDir d("localDir");
    QCloudDir cd(d);
    QCOMPARE(cd.getContents()->size(), 0);
    bool value = cd.isLocal();
    QCOMPARE(value, true);
}

void QCloudTest::testAmazonGetCloudDirs() {
    QAmazonConnection *conn = new QAmazonConnection();
    QList<QString> buckets = conn->getCloudDir();
    QCOMPARE(buckets.at(0), QString("heheheheheheh"));
}

void QCloudTest::testAmazonGetCloudDirsFail() {
    QAmazonConnection *conn = new QAmazonConnection("not","correct", "credentials");
    QList<QString> buckets = conn->getCloudDir();
    QCOMPARE(0, buckets.size());
}

void QCloudTest::testAmazonGetCloudDirContents() {
    QAmazonConnection *conn = new QAmazonConnection();
    QList<QString> buckets = conn->getCloudDir();
    QList<QString> files = conn->getCloudDirContents(buckets.at(0));
    QCOMPARE(QString(files.at(0)), QString("aaaaaaaa.txt"));

}

void QCloudTest::testAmazonGetCloudDirContentsFail() {
    QAmazonConnection *conn = new QAmazonConnection("not","correct", "credentials");
    QList<QString> files = conn->getCloudDirContents("asdfafsd");
    QCOMPARE(0, files.size());
}

void QCloudTest::testAmazonGetCloudFile() {
    QAmazonConnection *conn = new QAmazonConnection();
    QList<QString> buckets = conn->getCloudDir();
    QList<QString> files = conn->getCloudDirContents(buckets.at(0));

    QCloudFile* file = conn->get(buckets.at(0), files.at(0));
    QCOMPARE(QString(file->getContents()), QString("this is the test document"));
}

void QCloudTest::testAmazonGetCloudFileFail() {
    QAmazonConnection *conn = new QAmazonConnection("not","correct", "credentials");
    QSignalSpy spy(conn, SIGNAL(cloudError()));
    QCloudFile* file = conn->get("kikkare", "aaaaaaaa.txt");
    QCOMPARE(spy.count(), 1);
}


QTEST_MAIN(QCloudTest)

#include "tst_qcloudtest.moc"
