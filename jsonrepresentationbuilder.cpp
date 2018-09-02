#include "jsonrepresentationbuilder.h"

JsonRepresentationBuilder::JsonRepresentationBuilder(){}

void JsonRepresentationBuilder::BuildJsonDocumentFromRootItem
       (JsonItem *rootJsonItem, QJsonDocument &doc)
{
    QJsonObject *obj = new QJsonObject();
    JsonRepresentationBuilder::buildJsonDocument(rootJsonItem, obj);
    doc.setObject(*obj);
    delete obj;
}

void JsonRepresentationBuilder::buildJsonDocument(JsonItem *jsonItem, QJsonObject *obj)
{
    if (jsonItem->parentItem() != nullptr) {
        // add items at the root level
        obj->insert(jsonItem->key(), jsonItem->jsonValue);
    } else {
        for (int i = 0; i < jsonItem->childCount(); ++i) {
            JsonItem *child = jsonItem->child(i);

            if (child->jsonValue.isObject()) {
                JsonRepresentationBuilder::addChildItemsForJsonObjectRecursively(child, obj);
            } else {
                JsonRepresentationBuilder::buildJsonDocument(jsonItem->child(i), obj);
            }
        }
    }
}

void JsonRepresentationBuilder::addChildItemsForJsonObjectRecursively(JsonItem *jsonItem, QJsonObject *obj)
{
    QJsonObject *newObject = new QJsonObject();
    for (int j = 0; j < jsonItem->childCount(); ++j) {
        JsonItem *child = jsonItem->child(j);

        if (child->jsonValue.isObject()) {
            JsonRepresentationBuilder::addChildItemsForJsonObjectRecursively(child, newObject);
        } else {
            newObject->insert(jsonItem->child(j)->key(), jsonItem->child(j)->jsonValue);
        }
    }

    QJsonValue newValue(QJsonValue::Object);
    newValue = QJsonValue::fromVariant(newObject->toVariantMap());

    obj->insert(jsonItem->key(), newValue);
    delete newObject;
}
