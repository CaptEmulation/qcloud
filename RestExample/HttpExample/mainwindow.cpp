#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QLabel>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::finishedSlot(QNetworkReply* reply)
{
    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {

        // Example 2: Reading bytes form the reply
        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string
        QLabel *label = new QLabel("asdfasdf",this);
        label->setText(string);
    }
    // Some http error received
    else
    {
        // handle errors here
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    reply->deleteLater();
}

void MainWindow::on_button_clicked()
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this , SLOT(finishedSlot(QNetworkReply*)));

    QUrl url("http://sisuguild.fi/~jage/haha.hmtl");
    QNetworkReply *reply = nam->get(QNetworkRequest(url));

    //qDebug() << reply->bytesWritten();
}
