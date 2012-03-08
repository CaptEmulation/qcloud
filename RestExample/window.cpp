#include "window.h"
#include "ui_window.h"
#include <QDebug>
#include "resthandler.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>

/**
  Author: Jarkko Laitinen
  This is a really really alpha version of the software.
  */

/**
  Works on a hard coded values atm, will need to do alot of refactoring to generalize the whole thingy. Can now upload a file to a certain bucket in amazon s3
  and retrieve the same file.
  TODO:
      Generalization of the requests.
      Refactoring of the whole RestHandler class.
      Learning the 'right' way of doing the returning of QStrings and QByteArrays (pointers?)
      Alot of general learning of the c++ way of doing things coming from Java.
      Does not include ANY error handling so that should be taken care of.

  The version that is in git will not contain my own secretKey and authkey because I think that I don't want to share em.

  */

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    ui->bar->setMinimum(0);
    ui->bar->setValue(0);
}

Window::~Window()
{
    delete ui;
}

/**
  Initial version of the loading from cloud function.
  Todo:
    somehow generalizing the sending and receiving so that there would not be this much repetition.
  */
void Window::on_button_clicked()
{    
    RestHandler *hr = new RestHandler();
    qDebug() << "made the resthandler";

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    qDebug() << "making the connections";

    QString bucket = "kikkare";
    QNetworkReply *reply;
    QUrl url;
    url.setUrl(hr->makeGetUrl());
    QNetworkRequest bar(url);


    connect(manager, SIGNAL(finished(QNetworkReply *)), SLOT(slotRequestFinished(QNetworkReply *)));
    qDebug() << "sending packet: " << bar.url();
    reply = manager->get(bar);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(slotSetProgress(qint64,qint64)));

}
/**
  Spaghetticode version of the uploading of files.
  */
void Window::on_sendButton_clicked()
{
    qDebug() << "uploading the file to amazon cloud";

    RestHandler *hr = new RestHandler();
    QFile file("text.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply;
    QTextStream out(&file);
    out << ui->textBox->toPlainText();
    file.close();
    QUrl url(hr->makePutRequest(&file));
    QNetworkRequest request(url);
    file.open(QIODevice::ReadOnly);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotRequestFinished(QNetworkReply*)));

    reply = manager->put(request, file.readAll());

}

void Window::slotRequestFinished(QNetworkReply *reply){
    qDebug() << "slotrequestfinished";
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
      }
      else {
        QString foo = QString::fromUtf8(reply->readAll());
        ui->textBox->setText(foo);
      }
    reply->deleteLater();
}



void Window::slotSetProgress(qint64 rec, qint64 tot){
    qDebug() << "From slotSetProgress()";

    ui->bar->setMaximum(tot);
    ui->bar->setValue(rec);
}
