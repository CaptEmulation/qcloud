#include "settingsmenu.h"

#include <QtGui>
#include <QSettings>

QSettings set("JYU", "CloudExample");

/**
  creates a dialog that contains fields where the user can input login details.
  */
settingsmenu::settingsmenu(QWidget *parent) : QDialog(parent)
{
    QString un, pass, ha;
    bool hasSettings = (set.contains("username")) ? true : false;
    if(hasSettings) {
        un = set.value("username").toString();
        pass = set.value("password").toString();
        ha = set.value("hash").toString();
    } else {
        un = "username";
        pass = "password";
        ha = "hash";
    }

    username = new QLineEdit(un);

    password = new QLineEdit(pass);
    password->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    hash = new QLineEdit(ha);
    hash->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    cancel = new QPushButton(tr("Cancel"));
    ok = new QPushButton(tr("Save"));

    connect(ok, SIGNAL(clicked()), this, SLOT(settingsUpdated()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(username);
    inputLayout->addWidget(password);
    inputLayout->addWidget(hash);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(cancel);
    mainLayout->addWidget(ok);


    setLayout(mainLayout);

    setWindowTitle(tr("Settings"));
}

void settingsmenu::settingsUpdated(){

    set.setValue("username", username->text());
    set.setValue("password", password->text());
    set.setValue("hash", hash->text());
    emit(foo());
    this->close();
}

void settingsmenu::cancelClicked() {
    this->close();
}

