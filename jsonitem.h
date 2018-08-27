#ifndef JSONITEM_H
#define JSONITEM_H
#include <QList>
#include <QVariant>
#include <QJsonValue>

class JsonItem
{
public:
    // this follows example from:
    // http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html
    explicit JsonItem(const QList<QVariant> &data, JsonItem *parentItem = 0);
    ~JsonItem();

    void appendChild(JsonItem *child);
    void insertChildAt(int index, JsonItem *item);
    void removeChild(JsonItem *item);
    void updateParentItem(JsonItem *newParent);

    int childCount() const;
    int columnCount() const;
    int row() const;

    JsonItem *child(int row);
    JsonItem *parentItem();

    QString key() const;

    QVariant data(int column) const;
    QJsonValue jsonValue; // TODO: this should be private and passed through ctor

private:
    QList<JsonItem*> m_childItems;
    QList<QVariant> m_itemData;

    JsonItem *m_parentItem;
};
#endif // JSONITEM_H
