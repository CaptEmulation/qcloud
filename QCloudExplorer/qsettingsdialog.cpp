#include "qsettingsdialog.h"

QSettingsDialog::QSettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QSettings s("jyu","qcloudexplorer");

    username = new QLineEdit();
    usernameLabel = new QLabel("Username");
    QVBoxLayout *unameLay = new QVBoxLayout();
    unameLay->addWidget(usernameLabel);
    unameLay->addWidget(username);

    authkey = new QLineEdit();
    authkeyLabel = new QLabel("Authkey");
    QVBoxLayout *authLayout = new QVBoxLayout();
    authLayout->addWidget(authkeyLabel);
    authLayout->addWidget(authkey);

    secretkey = new QLineEdit();
    secretkeyLabel = new QLabel("SecretKey");
    QVBoxLayout *secretLay = new QVBoxLayout();
    secretLay->addWidget(secretkeyLabel);
    secretLay->addWidget(secretkey);

    saveButton = new QPushButton();
    saveButton->setText("Save");
    connect(saveButton, SIGNAL(clicked()), SLOT(saveButtonClicked()));

    cancelButton = new QPushButton();
    cancelButton->setText("Cancel");
    connect(cancelButton, SIGNAL(clicked()), SLOT(cancelButtonClicked()));

    QHBoxLayout *amazonLayout = new QHBoxLayout();
    amazon = new QRadioButton();
    connect(amazon, SIGNAL(clicked()), SLOT(amazonClicked()));
    amazonLabel = new QLabel();
    amazonLabel->setText("Amazon");

    amazonLayout->addWidget(amazonLabel);
    amazonLayout->addWidget(amazon);

    QHBoxLayout *azureLayout = new QHBoxLayout();
    azure = new QRadioButton();
    connect(azure, SIGNAL(clicked()), SLOT(azureClicked()));
    azureLabel = new QLabel();
    azureLabel->setText("Azure");
    azureLayout->addWidget(azureLabel);
    azureLayout->addWidget(azure);

    QVBoxLayout *both = new QVBoxLayout();
    both->addLayout(amazonLayout);
    both->addLayout(azureLayout);

    buttonlayout = new QVBoxLayout();
    buttonlayout->addWidget(saveButton);
    buttonlayout->addWidget(cancelButton);


    if (s.contains("username")) {
        username->setText(s.value("username").toByteArray());
        authkey->setText(s.value("accesskey").toByteArray());
        secretkey->setText(s.value("secretkey").toByteArray());
        if (s.value("service") == "amazon") {
            amazon->setChecked(true);
        } else {
            azure->setChecked(true);
        }
    } else {
        username->setText("Username");
        authkey->setText("Authkey");
        secretkey->setText("SecretKey");
    }

    layout = new QHBoxLayout();
    layout->addLayout(both);
    layout->addLayout(unameLay);
    layout->addLayout(authLayout);
    layout->addLayout(secretLay);

    layout->addLayout(buttonlayout);

    QHBoxLayout *foo = new QHBoxLayout();
    overrideCloud = new QCheckBox();
    overrideCloud->setText("Override Cloud");
    if (s.contains("overrideCloud")) {
        overrideCloud->setChecked(true);
    }

    overrideLocal = new QCheckBox();
    overrideLocal->setText("Override Local");
    if (s.contains("overrideLocal")) {
        overrideLocal->setChecked(true);
    }

    foo->addWidget(overrideCloud);
    foo->addWidget(overrideLocal);

    QVBoxLayout *total = new QVBoxLayout();
    total->addLayout(layout);
    total->addLayout(foo);
    this->setLayout(total);
    this->show();
}

void QSettingsDialog::amazonClicked() {
    QSettings s("jyu", "qcloudexplorer");
    if (s.value("service") == "amazon") {
        authkey->setText(s.value("accesskey").toByteArray());
        secretkey->setText(s.value("secretkey").toByteArray());
        username->setText(s.value("username").toByteArray());
    } else {
        authkey->setText("AWSAccessId");
        secretkey->setText("SecretKey");
        username->setText("Username");
    }
}

void QSettingsDialog::azureClicked() {
    QSettings s("jyu", "qcloudexplorer");
    if (s.value("service") == "azure") {
        authkey->setText(s.value("accesskey").toByteArray());
        secretkey->setText(s.value("secretkey").toByteArray());
        username->setText(s.value("username").toByteArray().toBase64());
    } else {
        authkey->setText("StorageAccount");
        secretkey->setText("PrivateKey");
        username->setText("Url");
    }
}

void QSettingsDialog::saveButtonClicked() {
    QSettings s("jyu","qcloudexplorer");
    if (s.contains("username")) {
        s.clear();
    }
    s.setValue("username", username->text());
    s.setValue("accesskey", authkey->text());
    if (amazon->isChecked())
    {
        s.setValue("service", "amazon");
        s.setValue("secretkey", secretkey->text());
    } else {
        s.setValue("service", "azure");
        s.setValue("secretkey", secretkey->text());
    }
    if (overrideCloud->isChecked()) {
        s.setValue("overrideCloud", "true");
    }
    if (overrideLocal->isChecked()) {
        s.setValue("overrideLocal", "true");
    }

    this->close();
    emit settingsUpdated();
}

void QSettingsDialog::cancelButtonClicked() {
    emit cancelClicked();
    this->close();
}
