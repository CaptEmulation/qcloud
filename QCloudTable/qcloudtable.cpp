#include "qcloudtable.h"
#include "ui_qcloudtable.h"

#include <QTableView>
#include <QHBoxLayout>
#include "qcloudhelper.h"
#include <QDebug>

QCloudTable::QCloudTable(QDialog *parent) : QDialog(parent), ui(new Ui::QCloudTable)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("tableName");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("First name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Last name"));

    QTableView *view = new QTableView();
    view->setModel(model);
    view->resizeColumnsToContents();

    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);

    revertButton = new QPushButton(tr("Revert"));
    quitButton = new QPushButton(tr("Quit"));
    buttonBox = new QDialogButtonBox(Qt::Vertical);

    buttonBox->addButton(submitButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);


    connectButton = new QPushButton(tr("Connect"));
    label = new QLabel(tr("Connect to adress"));
    urlBox = new QLineEdit(tr("Server"));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(urlBox);
    topLayout->addWidget(connectButton);

    QHBoxLayout *middle = new QHBoxLayout;
    middle->addWidget(view);
    middle->addWidget(buttonBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middle);
    setLayout(mainLayout);
    connect(connectButton, SIGNAL(clicked()), SLOT(connectClicked()));
    connect(submitButton, SIGNAL(clicked()), SLOT(submitButtonClicked()));
    helper = new QCloudHelper();
    connect(helper, SIGNAL(finished(QNetworkReply*)), SLOT(finished(QNetworkReply*)));
}

void QCloudTable::connectClicked() {
    helper->get("", QCloudHelper::TABLE);
}

/**

  */
void QCloudTable::submitButtonClicked() {
    helper->put(*model);
}

void QCloudTable::finished(QNetworkReply *reply){
    qDebug() << reply->readAll();
}


QCloudTable::~QCloudTable()
{
    delete ui;
}
