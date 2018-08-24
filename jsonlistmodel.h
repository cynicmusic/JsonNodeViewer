#ifndef JSONLISTMODEL_H
#define JSONLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QFont>
#include <QLinkedList>
#include <QList>
#include <QJsonParseError>
#include <QModelIndex>
#include <jsonitem.h>
#include <QStringList>
#include <QMimeData>
#include <QCoreApplication>
#include <QMap>
#include <QSharedPointer>
#include <QBrush>


class JsonListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit JsonListModel(const QJsonObject &data, QObject *parent = 0);
    ~JsonListModel();

    QVariant data(const QModelIndex &index, int role) const override;
    //QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int col, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::DropActions supportedDropActions() const;

    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QMimeData * mimeData(const QModelIndexList &indexes) const;
    JsonItem * nodeForIndex(const QModelIndex &index) const;

    QList<QJsonValue> rootJsonValues;

    JsonItem * getRootItem() const;

    void addNewObject(QJsonValue &thisItem, JsonItem *child);

private:
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;

    JsonItem *rootItem;



};

#endif // JSONLISTMODEL_H
