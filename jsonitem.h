#ifndef JSONITEM_H
#define JSONITEM_H
#include <QList>
#include <QVariant>
#include <QDebug>
#include <QJsonValue>
#include <QMimeData>


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

    JsonItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    JsonItem *parentItem();

    void updateParentItem(JsonItem *newParent);

    QJsonValue jsonValue; // should be private and passed through ctor
    QString key();
private:
    QList<JsonItem*> m_childItems;
    QList<QVariant> m_itemData;

    JsonItem *m_parentItem;
};
#endif // JSONITEM_H
