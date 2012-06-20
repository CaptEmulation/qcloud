#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QSettings;
QT_END_NAMESPACE

class settingsmenu : public QDialog
{
    Q_OBJECT
public:
    explicit settingsmenu(QWidget *parent = 0);
    
signals:
    void okClicked();
    void foo();
public slots:
   void settingsUpdated();

private slots:
   void cancelClicked();
private:
    QLineEdit *username;
    QLineEdit *password;
    QLineEdit *hash;
    QPushButton *cancel;
    QPushButton *ok;
};

#endif // SETTINGSMENU_H
