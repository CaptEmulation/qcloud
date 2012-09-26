#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirModel>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDirModel *model = new QDirModel();
    ui->localDirectory->setModel(model);
    ui->localDirectory->setRootIndex(model->index(QDir::currentPath()));
    QSettings s("jyu", "qcloudexplorer");
    if (s.contains("service")) {
        if (s.value("service") == "amazon") {
            cloud = new QAmazonConnection(s.value("username").toByteArray(), s.value("accesskey").toByteArray(),
                                          s.value("secretkey").toByteArray());
        } else {
            cloud = new QAzureConnection(s.value("username").toByteArray(), s.value("accesskey").toByteArray(), s.value("secretkey").toByteArray());
        }
        connect(cloud, SIGNAL(cloudRequestFinished(QCloudResponse*)), this, SLOT(requestFinished(QCloudResponse*)));
        qDebug() << s.value("username").toByteArray();
        qDebug() << s.value("accesskey").toByteArray();
        qDebug() << s.value("secretkey").toByteArray();
    }
    else {
        ui->connectButton->setEnabled(false);
        ui->getButton->setEnabled(false);
        ui->putButton->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestFinished(QCloudResponse *resp) {
    QCloudResponse::RESPONSETYPE t = resp->getResponseType();
    qDebug() << QString("requestFinished() with type of %1").arg(t);

    switch (t) {
        case QCloudResponse::CLOUDDIR : {
        qDebug() << "in asdfasdfasdf";
        QByteArray data = resp->getResponse();
        QList<QString> contents = cloud->parseCloudDirListings(data);
        ui->cloudDirectory->setModel(new QStringListModel(contents));
        break;
    }
    case QCloudResponse::CLOUDDIRCONTENTS: {

        break;
    }
    case QCloudResponse::CLOUDFILE:
        break;
    }
}

void MainWindow::on_connectButton_clicked()
{
    QSettings s("jyu", "qcloudexplorer");
    if (!s.contains("username")) {
        ui->connectButton->setEnabled(false);
        settingsdialog = new QSettingsDialog();
        settingsdialog->setWindowTitle("Settings");
        connect(settingsdialog, SIGNAL(settingsUpdated()), SLOT(settingsUpdated()));
        connect(settingsdialog, SIGNAL(cancelClicked()), SLOT(cancelClicked()));
        settingsdialog->show();
    } else {
        qDebug() << s.value("service").toByteArray();
        QList<QString> list = cloud->getCloudDir();
        ui->cloudDirectory->setModel(new QStringListModel(list));
    }

}

void MainWindow::on_getButton_clicked()
{
    disconnect(cloud, 0, this, 0);
    QProgressDialog dialog(this);
    dialog.setLabelText("Downloading");
    connect(cloud, SIGNAL(setRange(int,int)), &dialog, SLOT(setRange(int,int)));
    connect(cloud, SIGNAL(valueChanged(int)), &dialog, SLOT(setValue(int)));
    connect(cloud, SIGNAL(getCloudDirFinished()), &dialog, SLOT(close()));
    connect(cloud, SIGNAL(getCloudDirFinished()), this, SLOT(updateLocal()));
    QString whatToGet = ui->cloudDirectory->selectionModel()->selectedIndexes().at(0).data().toString();

    QDir d(whatToGet);
    if (!d.exists()) {
        d.mkdir(".");
    }
    QCloudDir dir(d);
    dialog.show();
    cloud->get(dir);
}

void MainWindow::on_putButton_clicked()
{
    dialog = new QProgressDialog();
    dialog->setLabelText("Uploading");
    connect(cloud, SIGNAL(setRange(int,int)), dialog, SLOT(setRange(int,int)));
    connect(cloud, SIGNAL(valueChanged(int)), dialog, SLOT(setValue(int)));
    connect(cloud, SIGNAL(putCloudDirFinished()), dialog, SLOT(close()));
    connect(cloud, SIGNAL(putCloudDirFinished()), this, SLOT(updateCloud()));

    QString whatToPut = ui->localDirectory->selectionModel()->selectedIndexes().at(0).data().toString();
    QDir dir(whatToPut);
    QCloudDir cloudDir(dir);
    dialog->show();
    cloud->put(cloudDir);
}

void MainWindow::putRequestFinished() {

}

void MainWindow::getRequestFinished() {

}

void MainWindow::settingsUpdated() {
    QSettings s("jyu", "qcloudexplorer");
    ui->connectButton->setEnabled(true);

    if (s.value("service") == "amazon") {
        cloud = new QAmazonConnection(s.value("username").toByteArray(), s.value("accesskey").toByteArray(),
                                      s.value("secretkey").toByteArray());
    } else {
        cloud = new QAzureConnection(s.value("username").toByteArray(), QByteArray("SharedKey"), QByteArray::fromBase64(s.value("secretkey").toByteArray()));
    }

    bool overrideLocal = s.contains("overrideLocal");
    bool overrideCloud = s.contains("overrideCloud");

    cloud->setOverrideCloud(overrideCloud);
    cloud->setOverrideLocal(overrideLocal);

    connect(cloud, SIGNAL(cloudRequestFinished(QCloudResponse*)), this, SLOT(requestFinished(QCloudResponse*)));
    ui->getButton->setEnabled(true);
    ui->putButton->setEnabled(true);
}

void MainWindow::cancelClicked() {

}

void MainWindow::on_settingsButton_clicked()
{
    settingsdialog = new QSettingsDialog();
    settingsdialog->setWindowTitle("Settings");
    connect(settingsdialog, SIGNAL(settingsUpdated()), SLOT(settingsUpdated()));
    connect(settingsdialog, SIGNAL(cancelClicked()), SLOT(cancelClicked()));
    settingsdialog->show();
}

void MainWindow::updateLocal() {
    QDirModel *model = new QDirModel();
    ui->localDirectory->setModel(model);
    ui->localDirectory->setRootIndex(model->index(QDir::currentPath()));
}

void MainWindow::updateCloud() {

}
