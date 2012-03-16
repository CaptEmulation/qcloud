#include "dialog.h"
#include "ui_dialog.h"
#include <QSettings>

QSettings settings("RestExample", "RestHandler");

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText(settings.value("userName").toString());
    ui->lineEdit_2->setText(settings.value("authKey").toString());
    ui->lineEdit_3->setText(settings.value("secretKey").toString());
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_saveButton_clicked()
{

    settings.setValue("userName", ui->lineEdit->text());
    settings.setValue("authKey",ui->lineEdit_2->text());
    settings.setValue("secretKey",ui->lineEdit_3->text());
    this->accept();

}
