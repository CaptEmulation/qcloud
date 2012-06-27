#ifndef QCLOUDTABLE_H
#define QCLOUDTABLE_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "qcloudhelper.h"

namespace Ui {
class QCloudTable;
}

class QCloudTable : public QDialog
{
    Q_OBJECT
    
public:
    explicit QCloudTable(QDialog *parent = 0);
    ~QCloudTable();
    
private:
    Ui::QCloudTable *ui;
    QSqlTableModel *model;
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *urlBox;
    QPushButton *connectButton;
    QCloudHelper *helper;
private slots:
    void connectClicked();
    void finished(QNetworkReply*);
    void submitButtonClicked();
};



#endif // QCLOUDTABLE_H
