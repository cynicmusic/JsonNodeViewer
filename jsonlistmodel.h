#ifndef JSONLISTMODEL_H
#define JSONLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFont>
#include <QList>
#include <QModelIndex>
#include <QStringList>
#include <QMimeData>
#include <QCoreApplication>
#include <QMap>
#include <QBrush>
#include <jsonitem.h>
#include <QFile>


class JsonListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit JsonListModel(const QJsonObject &data, QObject *parent = 0);
    ~JsonListModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const;

    QModelIndex index(int row, int col, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    JsonItem * getRootItem() const;

    void addJsonToModelRecursively(QJsonValue &thisItem, JsonItem *parent);

private:
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;

    JsonItem *rootItem;

    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QMimeData * mimeData(const QModelIndexList &indexes) const;
    JsonItem * nodeForIndex(const QModelIndex &index) const;

    QList<QJsonValue> rootJsonValues;



};

#endif // JSONLISTMODEL_H
