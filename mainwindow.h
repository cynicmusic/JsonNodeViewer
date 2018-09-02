#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
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

public slots:
     // Public for unit tests. TODO: make protected and subclass MainWindow instead
    void SaveJsonDocumentToDisk( void );

private slots:
    void OnModelDataChanged(QModelIndex, QModelIndex);

private:
    void alert(QString message);
    void UpdateJsonInTextWidget();
    bool LoadJsonDocumentFromDisk(QJsonDocument &doc, QString &filename);

    JsonListModel *model;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
