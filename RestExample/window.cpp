#include "window.h"
#include "ui_window.h"
#include <QDebug>
#include "resthandler.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QSettings>
#include <QDomDocument>
#include <QXmlInputSource>
#include <QDomNodeList>



/**
  Author: Jarkko Laitinen
  This is a almost alpha version of the software.
  */

/**
  This program functions as a broker between the user and the amazon S3 cloud. When the program starts user first has to
  input his/her own amazon s3 information to the settings menu. After that the program gives availible files from a certain
  bucket (atm still hardcoded). Then the user inputs what file he wants to the "command prompt" and the program takes care
  of the rest.

  TODO:
      Learning the 'right' way of doing the returning of QStrings and QByteArrays (pointers?)
      Alot of general learning of the c++ way of doing things coming from Java.
      Does not include ANY error handling so that should be taken care of.
      Generalization of the 'path' so that user first sees his/her buckets and after that
       by choosing what bucket sees whats inside of it.
      Refactoring doRequest to return a QNetworkReply pointer, and placing the method in to the resthandler class

  When I get the Azure registration to work, I will try and see what needs to be changed to get the same kind of functions
  to azure.

  */


Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    ui->bar->setMinimum(0);
    ui->bar->setValue(0);
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(slotOpenSettingsDialog()));
    QSettings settings("RestExample", "RestHandler");

    if (settings.value("userName").toString() == "") {
        ui->sendButton->setEnabled(false);
        ui->button->setEnabled(false);
        ui->textBox->setText("Please insert your account info in preferences");
    }
}


Window::~Window(){delete ui;}

void Window::on_button_clicked() {
    doRequest(RestHandler::GET);
}

void Window::on_sendButton_clicked() {
    doRequest(RestHandler::PUT);
}

void Window::doRequest(RestHandler::REQUEST_TYPE type) {
    rh = new RestHandler();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply;

    QNetworkRequest request(rh->makeRequestUrl(type, ui->commandBox->text()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotRequestFinished(QNetworkReply*)));

    switch (type) {
    case (RestHandler::PUT): {
        QFile f(ui->commandBox->text());
        if (!f.open(QIODevice::ReadWrite | QIODevice::Text)){
            return;
        }
        QTextStream out(&f);
        out << ui->textBox->toPlainText();
        reply = manager->put(request,f.readAll());
        f.close();
        }
        break;

    case (RestHandler::GET):
        reply = manager->get(request);
        break;
    }
}

void Window::slotRequestFinished(QNetworkReply *reply) {
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
      }
      else {

        QByteArray foo = reply->readAll();

        QDomDocument doc;
        QXmlInputSource source;


        source.setData(foo);
        doc.setContent(source.data());

        QDomNodeList list = doc.elementsByTagName("Key");
        QByteArray sout;

        if (list.size() > 0){
            int i = 0;
            for (; i< list.size(); i++){
                sout.append(list.at(i).toElement().text() + "\n");
            }
        } else {
            sout = foo;
        }

        ui->textBox->setText(QString::fromUtf8(sout));
        }
   reply->deleteLater();
}

void Window::slotOpenSettingsDialog() {
    newDialog = new Dialog();
    connect(newDialog, SIGNAL(accepted()), this, SLOT(slotSettingsSaved()));
    newDialog->show();
}

void Window::slotSetProgress(qint64 rec, qint64 tot) {
    ui->bar->setMaximum(tot);
    ui->bar->setValue(rec);
}

void Window::slotSettingsSaved() {
    ui->sendButton->setEnabled(true);
    ui->button->setEnabled(true);
    ui->textBox->setText("");
    ui->bar->setMinimum(0);
    ui->bar->setValue(0);
}
