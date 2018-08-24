#include "jsonitem.h"

JsonItem::JsonItem(const QList<QVariant> &data, JsonItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

void JsonItem::appendChild(JsonItem *item)
{
    m_childItems.append(item);
}

void JsonItem::insertChildAt(int index, JsonItem *item)
{
    m_childItems.insert(index, item);
}

void JsonItem::removeChild(JsonItem *item)
{
    m_childItems.removeOne(item);
}

JsonItem *JsonItem::child(int row)
{
    return m_childItems.value(row);
}

int JsonItem::childCount() const
{
    return m_childItems.count();
}

int JsonItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<JsonItem*>(this));

    return 0;
}

int JsonItem::columnCount() const
{
    return m_itemData.count();
}

QVariant JsonItem::data(int column) const
{
    const int VALUE_COLUMN = 1;
    const int TYPE_COLUMN = 2;

    if (column == VALUE_COLUMN) {
        if (jsonValue.isString()) {
            return jsonValue.toString();
        }
        else if (jsonValue.isDouble()) {
            return jsonValue.toDouble();
        }
        else if (jsonValue.isBool()) {
            return jsonValue.toBool();
        }
        else if (jsonValue.isObject()) {
            return "------";
        }
    }
    else if (column == TYPE_COLUMN) {
        switch (m_itemData.value(column).toInt()) {
        case QJsonValue::Double:
            return "Double";
            break;
        case QJsonValue::String:
            return "String";
            break;
        case QJsonValue::Object:
            return "Object";
            break;
        case QJsonValue::Bool:
            return "Bool";
            break;
        default:
            return "Other";
        }
    }

    QVariant d = m_itemData.value(column);
    return d;
}

JsonItem *JsonItem::parentItem()
{
    return m_parentItem;
}

void JsonItem::updateParentItem(JsonItem *newParent) {
    m_parentItem = newParent;
}

QString JsonItem::key() {
    return QString(m_itemData.value(0).toString());
}

JsonItem::~JsonItem()
{
    qDeleteAll(m_childItems);
}

