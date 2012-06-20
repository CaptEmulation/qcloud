#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QTreeWidget>
#include <QDomDocument>
#include <QXmlStreamReader>

class XmlParser
{
public:
    XmlParser();
    void parseBuckets(QXmlStreamReader *&reader, QTreeWidget *&tree);
    void parseListing(QXmlStreamReader *&reader, QTreeWidgetItem *father);
private:
};

#endif // XMLPARSER_H
