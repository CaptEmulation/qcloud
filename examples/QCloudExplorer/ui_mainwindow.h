/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Nov 6 16:46:38 2012
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QListView *cloudDirectory;
    QListView *localDirectory;
    QPushButton *putButton;
    QPushButton *getButton;
    QPushButton *connectButton;
    QLabel *cloudLabel;
    QLabel *localLabel;
    QPushButton *settingsButton;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(695, 476);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        cloudDirectory = new QListView(centralWidget);
        cloudDirectory->setObjectName(QStringLiteral("cloudDirectory"));
        cloudDirectory->setGeometry(QRect(450, 20, 231, 401));
        localDirectory = new QListView(centralWidget);
        localDirectory->setObjectName(QStringLiteral("localDirectory"));
        localDirectory->setGeometry(QRect(10, 20, 251, 401));
        putButton = new QPushButton(centralWidget);
        putButton->setObjectName(QStringLiteral("putButton"));
        putButton->setGeometry(QRect(270, 320, 171, 41));
        getButton = new QPushButton(centralWidget);
        getButton->setObjectName(QStringLiteral("getButton"));
        getButton->setGeometry(QRect(270, 370, 171, 41));
        connectButton = new QPushButton(centralWidget);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setGeometry(QRect(270, 270, 171, 41));
        cloudLabel = new QLabel(centralWidget);
        cloudLabel->setObjectName(QStringLiteral("cloudLabel"));
        cloudLabel->setGeometry(QRect(540, 0, 51, 16));
        localLabel = new QLabel(centralWidget);
        localLabel->setObjectName(QStringLiteral("localLabel"));
        localLabel->setGeometry(QRect(100, 0, 46, 13));
        settingsButton = new QPushButton(centralWidget);
        settingsButton->setObjectName(QStringLiteral("settingsButton"));
        settingsButton->setGeometry(QRect(270, 220, 171, 41));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 695, 21));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QCloud file transfer", 0));
        putButton->setText(QApplication::translate("MainWindow", "->", 0));
        getButton->setText(QApplication::translate("MainWindow", "<-", 0));
        connectButton->setText(QApplication::translate("MainWindow", "Connect", 0));
        cloudLabel->setText(QApplication::translate("MainWindow", "Cloud", 0));
        localLabel->setText(QApplication::translate("MainWindow", "Local", 0));
        settingsButton->setText(QApplication::translate("MainWindow", "Settings", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
