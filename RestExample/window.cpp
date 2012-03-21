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
  This is a really alpha version of the software.
  */

/**
  Works on a hard coded values atm, will need to do alot of refactoring to generalize the whole thingy. Can now upload a file to a certain bucket in amazon s3
  and retrieve the same file.
  TODO:
      Learning the 'right' way of doing the returning of QStrings and QByteArrays (pointers?)
      Alot of general learning of the c++ way of doing things coming from Java.
      Does not include ANY error handling so that should be taken care of.
  */
/**
  Now includes settings menu where one can enter his/her own amazon s3 credentials and then access the bucket.
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


Window::~Window()
{
    delete ui;
}


void Window::slotOpenSettingsDialog()
{
    newDialog = new Dialog();
    connect(newDialog, SIGNAL(accepted()), this, SLOT(slotSettingsSaved()));
    newDialog->show();
}

void Window::on_button_clicked()
{
    doRequest(RestHandler::GET);
}
void Window::on_sendButton_clicked()
{
    doRequest(RestHandler::PUT);
}

void Window::doRequest(RestHandler::REQUEST_TYPE type) {
    RestHandler *hr = new RestHandler();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QNetworkReply *reply;
    QFile f(ui->commandBox->text());

    if (type == RestHandler::PUT) {
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text)){
            return;
        }
        QTextStream out(&f);
        out << ui->textBox->toPlainText();
        f.close();
    }

    QNetworkRequest request(hr->makeRequestUrl(type, ui->commandBox->text()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotRequestFinished(QNetworkReply*)));

    if (type == RestHandler::PUT)
    {
        reply = manager->put(request, f.readAll());
    }
    if (type == RestHandler::GET)
    {
        //connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(slotSetProgress(qint64,qint64)));
        reply = manager->get(request);
    }
}

void Window::slotRequestFinished(QNetworkReply *reply){
    QFile f("text.txt");
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
        qDebug() << reply->readAll();
      }
      else {
        if (!f.open(QIODevice::ReadWrite)){
            qDebug() << "Problems opening file";
            return;
        }
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

        f.close();
      }
   reply->deleteLater();
}



void Window::slotSetProgress(qint64 rec, qint64 tot){
    ui->bar->setMaximum(tot);
    ui->bar->setValue(rec);
}

void Window::slotSettingsSaved(){
    ui->setupUi(this);
    ui->sendButton->setEnabled(true);
    ui->button->setEnabled(true);
    ui->textBox->setText("");
    ui->bar->setMinimum(0);
    ui->bar->setValue(0);
}
