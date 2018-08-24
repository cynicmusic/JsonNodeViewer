#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect( ui->save_document, SIGNAL(clicked()), this, SLOT(save_document()));

    bool success = load_json_file_from_disk("json.txt");
    if (!success) return;

    model = new JsonListModel(rootJsonObject);

    treeView = new QTreeView(this); // TODO: add treeView to .ui file
    treeView->resize(this->width()/2 + 100,450);
    treeView->setModel(model);
    treeView->setDragEnabled(true);
    treeView->setDragDropMode(QAbstractItemView::InternalMove);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);
    treeView->setAlternatingRowColors(true);
    treeView->viewport()->setAcceptDrops(true);

    treeView->setColumnWidth(0, 225);
    treeView->setColumnWidth(1, 125);
    treeView->setColumnWidth(2, 85);
    treeView->expandAll();

    connect ( model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(on_model_dataChanged(QModelIndex, QModelIndex)));

    update_json_document();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void alert(QString message) {
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

bool MainWindow::update_json_document(bool alsoSaveToFile)
{
    if (model) {
        JsonItem * rootJsonItem = model->getRootItem();
        QJsonObject *obj = new QJsonObject();
        buildJsonDocument(rootJsonItem, obj);

        QJsonDocument doc(*obj);
        ui->textEdit->setText(doc.toJson());
        if (alsoSaveToFile) {
            write_json_to_disk(doc, "output.txt");
        }
        delete obj;
    }
    return true;
}

bool MainWindow::write_json_to_disk(QJsonDocument &doc, const QString filename)
{
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        alert("Couldn't open save file " + filename);
        return false;
    }

    if (saveFile.write(doc.toJson()) != -1) {
        return true;
    } else {
        alert("Error saving file");
        return false;
    }
    return true;
}

void MainWindow::save_document( void )
{
    ui->save_document->setEnabled(false);
    bool success = update_json_document(true);
    if (success) {
        alert("Saved file to output.txt");
    }
    ui->save_document->setEnabled(true);
}

bool MainWindow::load_json_file_from_disk(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray ba = file.readAll();
        QJsonParseError jsonParseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &jsonParseError);
        if(jsonParseError.error != QJsonParseError::NoError) {
            alert("Parse Error loading JSON file!");
            return false;
        } else {
            rootJsonObject = jsonDoc.object();
        }
    } else {
        alert("File " + filename + "not found!");
        return false;
    }
    return true;
}

void MainWindow::buildJsonDocument(JsonItem *jsonItem, QJsonObject *obj, int level)
{
    if (level > 0) { // TODO: improve this
        obj->insert(jsonItem->key(), jsonItem->jsonValue);
    }

    if (jsonItem->childCount() > 0 && level == 0) {
        for (int i = 0; i < jsonItem->childCount(); ++i) {

            JsonItem *child = jsonItem->child(i);

            if (child->jsonValue.isObject()) {
                addChildItemsForJsonObjectRecursively(child, obj);
            }

            else {
                buildJsonDocument(jsonItem->child(i), obj, ++level);
            }
        }
    }
}

void MainWindow::addChildItemsForJsonObjectRecursively(JsonItem *jsonItem, QJsonObject *obj)
{
    QJsonObject *newObject = new QJsonObject();
    for (int j = 0; j < jsonItem->childCount(); ++j) {

        JsonItem *child = jsonItem->child(j);

            if (child->jsonValue.isObject()) {
                addChildItemsForJsonObjectRecursively(child, newObject);
            }

            else {
                newObject->insert(jsonItem->child(j)->key(), jsonItem->child(j)->jsonValue);
            }
    }

    QJsonValue newValue(QJsonValue::Object);
    newValue = QJsonValue::fromVariant(newObject->toVariantMap());

    obj->insert(jsonItem->key(), newValue);
    delete newObject;
}

void MainWindow::on_model_dataChanged(QModelIndex m, QModelIndex mi)
{
    Q_UNUSED(m);
    Q_UNUSED(mi);
    //qDebug() << "model data changed!";
    update_json_document();
    treeView->expandAll();
}
