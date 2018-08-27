#ifndef JSONREPRESENTATIONBUILDER_H
#define JSONREPRESENTATIONBUILDER_H
#include "jsonitem.h"
#include <QJsonObject>
#include <QJsonDocument>

class JsonRepresentationBuilder
{
public:
    JsonRepresentationBuilder();
    ~JsonRepresentationBuilder();

    static void BuildJsonDocumentFromRootItem(JsonItem *rootJsonItem, QJsonObject *obj, QJsonDocument &doc);

private:
    static void buildJsonDocument(JsonItem *jsonItem, QJsonObject *obj);
    static void addChildItemsForJsonObjectRecursively(JsonItem *jsonItem, QJsonObject *obj);
};

#endif // JSONREPRESENTATIONBUILDER_H
