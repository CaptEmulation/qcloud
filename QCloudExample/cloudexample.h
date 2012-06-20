#ifndef CLOUDEXAMPLE_H
#define CLOUDEXAMPLE_H

#include <QDialog>
#include <QHash>
#include <QNetworkAccessManager>
QT_BEGIN_NAMESPACE
class CloudExample;

class QDialogButtonBox;
class QFile;
class QFtp;
class QLabel;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QProgressDialog;
class QPushButton;
class QUrlInfo;
class QNetworkSession;
class settingsmenu;
class QCloudHelper;
class QDomNode;
class QModelIndex;
class XmlParser;
class QXmlInputSource;
QT_END_NAMESPACE


class CloudExample : public QDialog
{
    Q_OBJECT
    
public:
    explicit CloudExample(QWidget *parent = 0);
    ~CloudExample();
private slots:
    void connectOrDisconnect();
    void downloadFile();
    void populateTree(QXmlInputSource &input);
    void processItem(QTreeWidgetItem *item, int column);
    void enableDownloadButton();
    void showSettings();
    void settingsOK();
    void requestFinished(QNetworkReply*);
    void fileDownloaded(QByteArray);
    void fileListCollapsed(QTreeWidgetItem*);
    void fileListExpanded(QTreeWidgetItem*);
private:
    QString fileName;
    QList<QString> checked;
    CloudExample *ui;
    QLabel *ftpServerLabel;
    QLineEdit *ftpServerLineEdit;
    QLabel *statusLabel;
    QTreeWidget *fileList;
    QPushButton *connectButton;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QProgressDialog *progressDialog;
    QFile *file;
    QPushButton *settingsButton;
    settingsmenu *settings;
    QCloudHelper *helper;
    QNetworkAccessManager *manager;
    XmlParser *parser;
};

#endif // CLOUDEXAMPLE_H
