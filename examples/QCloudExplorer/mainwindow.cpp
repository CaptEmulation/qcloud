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
    QProgressDialog dialog(this);
    dialog.setLabelText("Downloading");
    connect(cloud, SIGNAL(setRange(int,int)), &dialog, SLOT(setRange(int,int)));
    connect(cloud, SIGNAL(valueChanged(int)), &dialog, SLOT(setValue(int)));
    connect(cloud, SIGNAL(getCloudDirFinished()), &dialog, SLOT(close()));
    connect(cloud, SIGNAL(getCloudDirFinished()), this, SLOT(updateLocal()));
    QString whatToGet = ui->cloudDirectory->selectionModel()->selectedIndexes().at(0).data().toString();


    QCloudDir dir(whatToGet);

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
    QFileInfo info(whatToPut);
    qDebug() << info.absolutePath();
    if(info.isFile()) {
        if (ui->cloudDirectory->selectionModel()->selectedIndexes().size() == 0) {
            QList<QString> list = cloud->getCloudDir();
            QFile f(whatToPut);
            QCloudFile fa(f);
            QString bucket = list.at(0);
            cloud->put(fa, bucket);
        } else {
            QString whereToPut = ui->cloudDirectory->selectionModel()->selectedIndexes().at(0).data().toString();
            QFile f(whatToPut);
            QCloudFile fa(f);
            cloud->put(fa, whereToPut);
        }
    } else {
        QDir dir(whatToPut);
        QCloudDir cloudDir(dir);
        dialog->show();
        cloud->put(cloudDir);
    }

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
    QList<QString> list = cloud->getCloudDir();
    ui->cloudDirectory->setModel(new QStringListModel(list));
}
