#include "xmlparser.h"

#include <QXmlStreamReader>
#include <QDebug>
#include <QXmlInputSource>
#include <QPixmap>

/**
  Goes through the doc and places it in the tree.
  */
XmlParser::XmlParser() {}

/**
  Parses files from a listing and places them as the child of their father (bucket they recide in)
  */
void XmlParser::parseListing(QXmlStreamReader *&reader, QTreeWidgetItem *father) {
    QTreeWidgetItem *itam;
    while (!reader->atEnd() && !reader->hasError()) {
        if (reader->name().toString().compare("Key", Qt::CaseInsensitive) == 0) {
               itam = new QTreeWidgetItem();
               QString text = reader->readElementText();
               itam->setText(0, text);

               //tähän parempi kikkare, jotta bucketin sisällä olevat kansiot erottuisivat jotenkin paremmin. esim vaikka niissä olevat
               //tiedostot olisivat kansion lapsia. ATM UGLY HACK
               QString pic = (text.contains("/")) ? (!text.contains(".")) ? ":/images/dir.png" : ":/images/file.png" : ":/images/file.png";
               QPixmap pixmap(pic);
               //END UGLY HACK

               itam->setIcon(0,pixmap);
               father->addChild(itam);
               qDebug() << "Added child to " << father->text(0) << " with properties " << itam->text(0);
        }
        reader->readNextStartElement();
    }
}

/**
  Parses buckets from the QXmlStreamReader to the QTreeWidget.
  */
void XmlParser::parseBuckets(QXmlStreamReader *&reader, QTreeWidget *&tree) {
    QTreeWidgetItem *bucket;
    while (!reader->atEnd() && !reader->hasError())
        if (reader->name().toString().compare("Name", Qt::CaseInsensitive) == 0) {
            bucket = new QTreeWidgetItem();
            bucket->setText(0,reader->readElementText());
            QPixmap pixmap(":/images/dir.png");
            bucket->setIcon(0,pixmap);
            bucket->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
            tree->addTopLevelItem(bucket);
            reader->readNextStartElement();
        } else {
            reader->readNextStartElement();
        }

}

