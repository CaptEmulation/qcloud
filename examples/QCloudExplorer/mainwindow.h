#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QtWidgets/QMainWindow>
//#include <QtWidgets/QProgressDialog>
//#include <QtWidgets/QDialog>
#include <QMainWindow>
#include <QProgressDialog>
#include <QDialog>
#include <QDebug>
#include <QSettings>
#include "qamazonconnection.h"
#include "qcloudresponse.h"
#include "qclouddir.h"
#include "qazureconnection.h"
#include "qsettingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCloudConnection *cloud;
    QProgressDialog *dialog;
    QSettingsDialog *settingsdialog;
private slots:
    void requestFinished(QCloudResponse*);
    void on_connectButton_clicked();
    void on_getButton_clicked();
    void on_putButton_clicked();
    void putRequestFinished();
    void getRequestFinished();
    void settingsUpdated();
    void cancelClicked();
    void on_settingsButton_clicked();
    void updateLocal();
    void updateCloud();
};

#endif // MAINWINDOW_H
