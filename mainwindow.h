#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QListView>
#include <QTreeView>
#include "jsonlistmodel.h"
#include "jsonitem.h"
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    JsonListModel * model;
    QTreeView * treeView; // make this private?

private slots:
    void save_document( void );
    void on_model_dataChanged(QModelIndex, QModelIndex);

private:
    Ui::MainWindow *ui;

    void buildJsonDocument(JsonItem *jsonItem, QJsonObject *obj, int level = 0);
    void addChildItemsForJsonObject(JsonItem *jsonItem, QJsonObject *obj);

    QJsonObject rootJsonObject;

    bool load_json_file_from_disk(QString filename);
    bool write_json_to_disk(QJsonDocument &doc, const QString filename);
    bool update_json_document(bool alsoSaveToFile = false);
};

#endif // MAINWINDOW_H
