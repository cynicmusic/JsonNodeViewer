#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QListView>
#include <QTreeView>
#include <QDebug>
#include "jsonlistmodel.h"
#include "jsonitem.h"
#include "jsonfileio.h"
#include "jsonrepresentationbuilder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void SaveJsonDocumentToDisk( void );
    void OnModelDataChanged(QModelIndex, QModelIndex);

private:
    Ui::MainWindow *ui;
    JsonListModel * model;
    void buildJsonDocument(JsonItem *jsonItem, QJsonObject *obj, int level = 0);
    void UpdateJsonInTextWidget();
};

#endif // MAINWINDOW_H
