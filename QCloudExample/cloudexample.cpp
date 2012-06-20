#include "cloudexample.h"
#include "settingsmenu.h"
#include "qcloudhelper.h"

#include "xmlparser.h"
#include <QtGui>
#include <QSettings>
#include <QXmlInputSource>
#include <QDomNodeList>
#include <QNetworkReply>
#include <QStringList>


CloudExample::CloudExample(QWidget *parent) : QDialog(parent)
{
    QSettings settings("JYU", "CloudExample");
    bool hasSettings = (settings.contains("username")) ? true : false;

    ftpServerLabel = new QLabel(tr("Connect &server:"));
    ftpServerLineEdit = new QLineEdit("s3.amazonaws.com");
    ftpServerLabel->setBuddy(ftpServerLineEdit);

    if (!hasSettings) {
        statusLabel = new QLabel(tr("Please update your user information in settings"));
    } else {
        statusLabel = new QLabel(tr("Please enter the URL of the cloud service"));
    }
    fileList = new QTreeWidget;
    fileList->setEnabled(false);
    fileList->setRootIsDecorated(false);
    fileList->setHeaderLabels(QStringList() << tr("Name") << tr("Size") << tr("Owner"));
    fileList->header()->setStretchLastSection(false);
    fileList->setRootIsDecorated(true);

    connectButton = new QPushButton(tr("Connect"));
    connectButton->setEnabled(hasSettings);

    downloadButton = new QPushButton(tr("Download"));
    downloadButton->setEnabled(false);

    settingsButton = new QPushButton(tr("Settings"));
    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(settingsButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    progressDialog = new QProgressDialog(this);

    connect(fileList, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(processItem(QTreeWidgetItem*,int)));
    connect(fileList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(enableDownloadButton()));
    connect(fileList, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(fileListExpanded(QTreeWidgetItem*)));
    connect(fileList,SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(fileListCollapsed(QTreeWidgetItem*)));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectOrDisconnect()));
    connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadFile()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(showSettings()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(ftpServerLabel);
    topLayout->addWidget(ftpServerLineEdit);
    topLayout->addWidget(connectButton);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);

    mainLayout->addWidget(fileList);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Cloud"));
}

void CloudExample::showSettings() {
    settings = new settingsmenu();
    connect(settings, SIGNAL(foo()), this, SLOT(settingsOK()));
    settings->show();
}

void CloudExample::settingsOK(){
    connectButton->setEnabled(true);
}


CloudExample::~CloudExample()
{
    delete ui;
}


void CloudExample::connectOrDisconnect(){
    helper = new QCloudHelper();
    connect(helper, SIGNAL(finished(QNetworkReply*)), SLOT(requestFinished(QNetworkReply*)));
    helper->get(ftpServerLineEdit->text());
}

/**
  This slot is connected to the QNetworkAccessManagers finished signal.
  */
void CloudExample::requestFinished(QNetworkReply *reply) {
    if(reply->error() > 0) {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
    }
    else {
        QByteArray foo = reply->readAll();
        QXmlInputSource source;
        source.setData(foo);

        //Ugly hack, checkes if the response from Amz is a XML formatted or not. If not the response is a file downloaded.
        if (!(foo.at(0) == '<')) {
            fileDownloaded(foo);
        } else {
            populateTree(source);
        }
        //End ugly hack
    }
    fileList->setEnabled(true);
    reply->deleteLater();
}

/**
  This slot is used when a bucket is expanded. The download of the file structures inside the bucket is done only
  once to save requests.
  */
void CloudExample::processItem(QTreeWidgetItem *item, int column) {
    QString name = item->text(0);
    if (!checked.contains(name)){
        helper->get(name + "." + ftpServerLineEdit->text());
    }
}

/**
  CloudExample::populateTree() is the main function that handles the forming of the QTreeWidgets tree structure.
  */
void CloudExample::populateTree(QXmlInputSource &input){
    QTreeWidgetItem *foo;
    QXmlStreamReader *reader = new QXmlStreamReader(input.data());

    if (parser == 0) {
        parser = new XmlParser();
    }

    reader->readNext();
    reader->readNext();

    if (reader->name().toString().compare("ListAllMyBucketsResult", Qt::CaseInsensitive) == 0) {
        parser->parseBuckets(reader, fileList);
    }
    else if (reader->name().toString().compare("ListBucketResult", Qt::CaseInsensitive) == 0) {
        reader->readNext();
        QString bucket = reader->readElementText();
        QList<QTreeWidgetItem *> matches = fileList->findItems(bucket, Qt::MatchCaseSensitive, 0);
        checked << bucket;
        if(matches.size() > 0) {
            foo = matches.at(0);
            parser->parseListing(reader, foo);
        }
        if (matches.size() > 0) { fileList->expandItem(matches.at(0));}
    }
}

/**
  This slot is connected to the downloadButtons clicked() signal. It gets the name of the selected file and gets request url from the
  cloudhelper. I think that the part between comments should be integrated to the cloudhelper as it is repeated in many places.
  */
void CloudExample::downloadFile(){
    fileName = fileList->currentItem()->text(0);
    statusLabel->setText(tr("Downloading file %1").arg(fileName));
    helper->get(fileList->currentItem()->parent()->text(0)+"."+ ftpServerLineEdit->text() + "/" + fileName);
}

/**
  This function gets a QByteArray that is the downloaded file. It makes a file with the same filename and places the array inside it.
  */
void CloudExample::fileDownloaded(QByteArray foo) {
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("QCloud"), tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString()));
        f.remove();
        return;
    }
    QTextStream out(&f);
    out << foo;
    f.close();
    statusLabel->setText(tr("%1 downloaded to default folder").arg(fileName));
}


/**
  Contains again a ugly hack. If a filename contains a dot it is considered to be a file, not a folder thus enabling the download
  button
  */
void CloudExample::enableDownloadButton(){

    QTreeWidgetItem* current = fileList->currentItem();
    if (current) {
        QString currentFile = current->text(0);
        QStringList list = currentFile.split(QRegExp("[./"));
        if (list.size() >= 2) {
            downloadButton->setEnabled(true);
        }
    }
    else downloadButton->setEnabled(false);
}

/**
  Nothing done in this slot at the moment
  */
void CloudExample::fileListCollapsed(QTreeWidgetItem *item) {}

/**
  If a bucket is "opened" using the thingy before it, it is populated using the same mechanism as it were double clicked.
  */
void CloudExample::fileListExpanded(QTreeWidgetItem *item) {
    processItem(item,0);
}


