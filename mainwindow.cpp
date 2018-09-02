#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::alert(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QJsonDocument doc;
    QString filename("json.txt");
    if (!LoadJsonDocumentFromDisk(doc, filename)) {
        return;
    }

    model = new JsonListModel(doc.object());

    ui->treeView->setModel(model);
    ui->treeView->setColumnWidth(0, 225);
    ui->treeView->setColumnWidth(1, 125);
    ui->treeView->setColumnWidth(2, 85);
    ui->treeView->expandAll();

    connect ( model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(OnModelDataChanged(QModelIndex, QModelIndex)));
    connect ( ui->save_document_button, SIGNAL(clicked()), this, SLOT(SaveJsonDocumentToDisk()));

    UpdateJsonInTextWidget();
}

bool MainWindow::LoadJsonDocumentFromDisk(QJsonDocument &doc, QString &filename)
{
    int errorCode;

    bool success = JsonFileIO::LoadJsonDocumentFromDisk(doc, filename, errorCode);
    if (!success) {
        if (errorCode == JsonFileIOError::CouldNotOpenFile) {
            alert("Error reading from file " + filename);
            return false;
        } else if (errorCode == JsonFileIOError::CouldNotParseFile) {
            alert("Error parsing JSON in " + filename);
            return false;
        }
        else {
            alert("Unknown error loading file");
            return false;
        }
    }
    return true;
}

void MainWindow::UpdateJsonInTextWidget()
{
    JsonItem *rootJsonItem = model->getRootItem();
    QJsonDocument doc;

    JsonRepresentationBuilder::BuildJsonDocumentFromRootItem(rootJsonItem, doc);

    ui->textEdit->setText(doc.toJson());
}

void MainWindow::SaveJsonDocumentToDisk( void )
{
    JsonItem *rootJsonItem = model->getRootItem();
    QJsonDocument doc;

    JsonRepresentationBuilder::BuildJsonDocumentFromRootItem(rootJsonItem, doc);

    ui->save_document_button->setEnabled(false);

    int errorCode;
    QString filename("output.txt");
    bool success = JsonFileIO::SaveJsonDocumentToDisk(doc, filename, errorCode);
    if (!success) {
        if (errorCode == JsonFileIOError::CouldNotSaveFile) {
            alert("Error saving file " + filename);
        } else {
            alert("Unknown error saving file");
        }
    }

    ui->save_document_button->setEnabled(true);
}

void MainWindow::OnModelDataChanged(QModelIndex m, QModelIndex mi)
{
    Q_UNUSED(m);
    Q_UNUSED(mi);
    UpdateJsonInTextWidget();
    ui->treeView->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}
