#ifndef WINDOW_H
#define WINDOW_H

#include "qmainwindow.h"
#include "qnetworkreply.h"
#include "dialog.h"

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Window(QWidget *parent = 0);
    ~Window();

    
public slots:
    void on_button_clicked();
    void slotRequestFinished(QNetworkReply *);
    void slotSetProgress(qint64, qint64);
    void slotOpenSettingsDialog();
    void slotSettingsSaved();

private slots:
    void on_sendButton_clicked();

private:
    Ui::Window *ui;
    Dialog *newDialog;
    QString userName;
    QString userKey;
    QString secretKey;
};

#endif // WINDOW_H
