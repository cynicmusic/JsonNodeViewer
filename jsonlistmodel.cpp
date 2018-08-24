#include "jsonlistmodel.h"
#include "jsonitem.h"

JsonItem * JsonListModel::getRootItem() const
{
    return rootItem;
}

JsonListModel::JsonListModel(const QJsonObject &data, QObject *parent)
    : QAbstractItemModel(parent),jsonObject(data)
{
    // Set column header and root data object
    QList<QVariant> rootData;
    rootData << "Key" << "Value" << "Type";
    rootItem = new JsonItem(rootData);
    //jsonObject = data;

    // iterate over QJsonObject and add objects
    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        QJsonValue jsonValue = data[it.key()];
        QList<QVariant> columnData;
        columnData << QString(it.key()) << jsonValue << jsonValue.type();

        JsonItem *child = new JsonItem(columnData,rootItem);
        child->jsonValue = jsonValue;
        rootItem->appendChild(child);

        if (jsonValue.isObject()) {
            addNewObject(jsonValue, child);
        }
    }
}

void JsonListModel::addNewObject(QJsonValue &thisItem, JsonItem *child) {
    QJsonObject newObject = thisItem.toObject();

     for (auto it1 = newObject.constBegin(); it1 != newObject.constEnd(); ++it1) {
         QJsonValue newItem = newObject[it1.key()];
         QList<QVariant> columnData1;
         columnData1 << QString(it1.key()) << newItem << newItem.type();

         JsonItem *child1 = new JsonItem(columnData1,child);
         child1->jsonValue = newItem;
         child->appendChild(child1);

         if (newItem.isObject()) {
             addNewObject(newItem, child1);
         }
     }
}




JsonListModel::~JsonListModel()
{
    qDebug() << "call destructor!!!!! =====================";
    delete rootItem;
}

//Since we cannot add data to the model after it is constructed and set up,
//this simplifies the way that the internal tree of items is managed.

QModelIndex JsonListModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    JsonItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<JsonItem*>(parent.internalPointer());

    JsonItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}



QModelIndex JsonListModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    JsonItem *childItem = static_cast<JsonItem*>(index.internalPointer());
    JsonItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int JsonListModel::rowCount(const QModelIndex &parent) const
{
    JsonItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<JsonItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int JsonListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<JsonItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant JsonListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::FontRole)
    {
        JsonItem *node = nodeForIndex(index);

        QFont font;
        font.setPointSize(14);

        if (node->jsonValue.isDouble()) {
            font.setItalic(true);
        }
        else if (node->jsonValue.isObject()) {
           font.setWeight(90);
        }
        else if (node->jsonValue.isString()) {
            font.setWeight(10);
        }
        return QVariant(font);
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    JsonItem *item = static_cast<JsonItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags JsonListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
}

static const char s_treeNodeMimeType[] = "application/x-treenode";

QStringList JsonListModel::mimeTypes() const
{
    return QAbstractItemModel::mimeTypes() << s_treeNodeMimeType;
}

QMimeData *JsonListModel::mimeData(const QModelIndexList &indexes) const
{
    // TODO: this new leaks memory, as in Qt examples as well.
    // TODO: Cache these mimeData objects
    QMimeData *mimeData = new QMimeData;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QList<JsonItem *> nodes;
    foreach (const QModelIndex &index, indexes) {
        JsonItem *node = nodeForIndex(index);
        if (!nodes.contains(node))
            nodes << node;
    }
    stream << QCoreApplication::applicationPid();
    stream << nodes.count();
    foreach (JsonItem *node, nodes) {
        stream << reinterpret_cast<qlonglong>(node);
    }
    mimeData->setData(s_treeNodeMimeType, data);

    return mimeData;
}



bool JsonListModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action)
    Q_UNUSED(row)
    Q_UNUSED(column)
     if (parent.isValid()) {
        // get target node we are dropping onto
        JsonItem *targetNode = nodeForIndex(parent);

        // convoluted method to get source node (the node which is being dragged)
        // https://forum.qt.io/topic/76708/full-drag-and-drop-support-in-qtreeview/2
        QByteArray ddata = data->data(s_treeNodeMimeType);
        QDataStream stream(&ddata, QIODevice::ReadOnly);
        qint64 senderPid;
        stream >> senderPid;
        int count;
        stream >> count;

        qlonglong nodePtr;
        stream >> nodePtr;
        JsonItem *sourceNode = reinterpret_cast<JsonItem *>(nodePtr);

        // Main procedure to move the SOURCE NODE to the parent of the TARGET NODE
        // this is not ideal because nested QJsonValue data gets out of sync
        //
        // In addition, forcing a reset of the model loses node selection in the tree view
        // and collapses all branches. Workarounds have been added to mainwindow.cpp
        // TODO: fix me so that model reset is not required, then remove workarounds
        beginInsertRows(parent,rowCount(parent), rowCount(parent));
        beginResetModel();

        // first, remove the source node from the source node's parent.
        sourceNode->parentItem()->removeChild(sourceNode);

        // next, add source node to the TARGET NODE's parent, at the TARGET NODE's index
        // Qt is generous about bounds checking here

        // if the TARGET NODE is an object, insert it directly into that node.
        if (targetNode->jsonValue.isObject()) {
            targetNode->insertChildAt(0, sourceNode);
            sourceNode->updateParentItem(targetNode); // update source node's parent item
        } else { // otherwise if TARGET NODE is a value
            targetNode->parentItem()->insertChildAt(targetNode->row(), sourceNode);
            sourceNode->updateParentItem(targetNode->parentItem()); // update source node's parent item
        }

        endInsertRows();
        endResetModel();

        emit dataChanged(parent, parent); // TODO: fixme -- possible workaround to call slot in mainwindow
        return true;
     }
    return false;
}

JsonItem * JsonListModel::nodeForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return rootItem;
    else
        return static_cast<JsonItem*>(index.internalPointer());
}

QVariant JsonListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}


Qt::DropActions JsonListModel::supportedDropActions() const
{
    return /*Qt::CopyAction |*/  Qt::MoveAction;
}



//bool JsonListModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    if (index.row() >= 0 && index.row() < rootJsonValues.size()
//        && (role == Qt::EditRole || role == Qt::DisplayRole)) {
//        qDebug() << "what is type " << typeid(value).name();
//        qDebug() << "what is variant type " << value.typeName();


//        // TODO major problem here! --- all this matching we're doing is based on QString...more complex objects will NOT match!

//        QJsonValue thisItem = QJsonValue::fromVariant(value);
//        int indexOfThisItem = rootJsonValues.indexOf(thisItem);
//        qDebug() << "index of this item " << indexOfThisItem;

//        int indexToInsert = index.row();
//        int indexToRemove = indexOfThisItem;

//        qDebug() << "insert at " << indexToInsert << " -- remove at " << indexToRemove;

//        if (indexToRemove >= 0) {
//            rootJsonValues.removeAt(indexToRemove); // remove needs to come first? but does that mess things up if you move down?
//        }
//        rootJsonValues.insert(indexToInsert, thisItem);

//        beginResetModel();

//        QVector<int> roles;
//        roles.reserve(2);
//        roles.append(Qt::DisplayRole);
//        roles.append(Qt::EditRole);
//        emit dataChanged(index, index, roles);
//        return true;
//    }
//    return false;
//}


//QModelIndex JsonListModel::index(int row, int col, const QModelIndex &parent) const
//{
//    if (!hasIndex(row, col, parent))
//        return QModelIndex();

//    // check if thing is valid like parent index etc

//    qDebug() << "creating index for " << row;

//    QJsonValue * alex = rootJsonValues.at(row);
//    return createIndex(row, 0, alex);


//    //return QAbstractItemModel::createIndex(row, 0, alex);
//    return QModelIndex();
//}

//QModelIndex JsonListModel::parent(const QModelIndex &child) const
//{
//    return QModelIndex();
//}






//QVariant JsonListModel::data(const QModelIndex &index, int role) const
//{
//    if (!index.isValid())
//            return QVariant();
//    if (index.row() >= rootJsonValues.size() || index.row() < 0)
//            return QVariant();

//    int row = index.row();

//    int *i = static_cast<int*>(index.internalPointer());
//    qDebug() << "is this data " << i;

//    QJsonValue jsonValue = rootJsonValues.at(row);


//    // THURSDAY FINAL DAY!!!
//    // THURSDAY FINAL DAY!!!
//    QJsonValue *thing = static_cast<QJsonValue *>(index.internalPointer());
//    //qDebug() << "what is type in data " << typeid(*thing).name() << "its a " << thing->toString();



//    // THURSDAY FINAL DAY!!!
//    // THURSDAY FINAL DAY!!!
//    // THURSDAY FINAL DAY!!!

//    if (role == Qt::DisplayRole)
//    {


//        if (jsonValue.isString()) {
//            return QString(jsonValue.toString());
//        }
//        else if (jsonValue.isDouble()) {
//            return QString::number(jsonValue.toDouble());
//        }else if (jsonValue.isBool()) {
//            return jsonValue.toBool() == 1 ? QString("true") : QString("false");
//        } else if (jsonValue.isArray()) {
//            return QString(jsonValue.toArray()[0].toString());
//        } else if (jsonValue.isObject()) {
//            return QString("{" + jsonValue.toObject().begin().value().toString() + "}");
//        }

//        return QString(rootJsonValues.at(row).toString());
//    }
//    if (role == Qt::FontRole)
//    {
//        {
//            QFont font;
//            font.setBold(true);
//            font.setPointSize(18);

//            if (jsonValue.isDouble()) {
//                qDebug() << "omg!!";
//                font.setItalic(true);
//            }

//            return font;
//        }
//    }
//    return QVariant();
//}

//bool JsonListModel::loadJsonFromDocument(const QJsonDocument &jsonDoc) { // should be loadJsonIntoModel

//    if (!jsonDoc.isNull()) {
//        beginResetModel();
//        //todo

//        this->jsonDocument = jsonDoc; // i think we need to dynamically allocate this

//        jsonObject = jsonDocument.object();

//    }
//    return true;
//}




//bool JsonListModel::loadFile(const QString &fileName)
//{
//    QFile file(fileName);
//    bool success = false;
//    if (file.open(QIODevice::ReadOnly)) {
//        //success = load(&file);

//        QByteArray ba = file.readAll();
//        QJsonParseError jsonParseError;
//        QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &jsonParseError);
//        if(jsonParseError.error != QJsonParseError::NoError) {
//            return false;
//        }


//        /////////////////// move this
//        QJsonObject rootJsonObject = jsonDoc.object();

//        qDebug() << "i love horses! " << rootJsonObject.length();


//        int i = 0;
//        for (auto it = rootJsonObject.constBegin(); it != rootJsonObject.constEnd(); ++it) {

//        QJsonValue thisItem = QJsonValue::fromVariant(*it);

//        qDebug() << "Creating data " << thisItem.toString();
//            rootJsonValues.append(*it);
//            qDebug() << "*it horse!! " << typeid(*it).name();

////            int * alex = new int(88);
////            createIndex(i, 0, alex);
//        }
//        //beginResetModel();




//        /////////////////

//        //loadJsonFromDocument(jsonDoc);
//        file.close();
//        qDebug() << "yep";
//        return true;
//    }
//    else success = false;
//    qDebug() << "nope";
//    return success;
//}



