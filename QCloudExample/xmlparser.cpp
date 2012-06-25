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
void XmlParser::parseListing(QXmlStreamReader &reader, QTreeWidgetItem *father) {
    QTreeWidgetItem *itam;
    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.name().toString().compare("Key", Qt::CaseInsensitive) == 0) {
               itam = new QTreeWidgetItem();
               QString text = reader.readElementText();
               itam->setText(0, text);

               //tähän parempi kikkare, jotta bucketin sisällä olevat kansiot erottuisivat jotenkin paremmin. esim vaikka niissä olevat
               //tiedostot olisivat kansion lapsia. ATM UGLY HACK
               QString pic = (text.contains("/")) ? (!text.contains(".")) ? ":/images/dir.png" : ":/images/file.png" : ":/images/file.png";
               //END UGLY HACK

               //Koska DisplayName tulee aina Sizen jälkeen voidaan silmukkaa ajaa niin kauan ja vasta breakata kun displayname ollaan laitettu
               //QTreeWidgetItemille. Täten saadaan tarvittavat tiedot. readerin ei tulisi ikinä mennä loppuun asti.

               while (!reader.atEnd()) {
                   reader.readNextStartElement();
                   if (reader.name().compare("Size",Qt::CaseInsensitive) == 0){
                        itam->setText(1,reader.readElementText());
                   }
                   else if (reader.name().compare("DisplayName",Qt::CaseInsensitive) == 0) {
                       itam->setText(2, reader.readElementText());
                       break;
                   }
               }
               QPixmap pixmap(pic);
               itam->setIcon(0,pixmap);
               father->addChild(itam);
        }
        reader.readNextStartElement();
    }
}

/**
  Parses buckets from the QXmlStreamReader to the QTreeWidget.
  */
void XmlParser::parseBuckets(QXmlStreamReader &reader, QTreeWidget &tree) {
    QTreeWidgetItem *bucket;
    while (!reader.atEnd() && !reader.hasError())
        if (reader.name().toString().compare("Name", Qt::CaseInsensitive) == 0) {
            bucket = new QTreeWidgetItem();
            bucket->setText(0,reader.readElementText());
            QPixmap pixmap(":/images/dir.png");
            bucket->setIcon(0,pixmap);
            bucket->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
            bucket->setText(1,"Bucket");
            tree.addTopLevelItem(bucket);
            reader.readNextStartElement();
        } else {
            reader.readNextStartElement();
        }

}

