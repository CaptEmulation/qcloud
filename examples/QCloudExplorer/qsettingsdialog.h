#ifndef QSETTINGSDIALOG_H
#define QSETTINGSDIALOG_H

/*
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
*/
#include <QDialog>
#include <QLineEdit>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>

#include <QSettings>
#include <QDebug>


class QSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QSettingsDialog(QWidget *parent = 0);

signals:
    void settingsUpdated();
    void cancelClicked();
private slots:
    void saveButtonClicked();
    void cancelButtonClicked();
    void amazonClicked();
    void azureClicked();
private:
    QLineEdit *username;
    QLineEdit *authkey;
    QLineEdit *secretkey;
    QLabel *usernameLabel;
    QLabel *authkeyLabel;
    QLabel *secretkeyLabel;

    QRadioButton *amazon;
    QRadioButton *azure;
    QLabel *amazonLabel;
    QLabel *azureLabel;
    QHBoxLayout *layout;
    QVBoxLayout *buttonlayout;
    QVBoxLayout *radio;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QCheckBox *overrideCloud;
    QCheckBox *overrideLocal;
    QCheckBox *onlyNewer;
};

#endif // QSETTINGSDIALOG_H
