#include "unittests.h"
#include <QDebug>
#include <QTest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "mainwindow.h"
#include "jsonrepresentationbuilder.h"
#include "jsonfileio.h"

void UnitTests::initTestCase() {
    // This function is being executed at the beginning of each test suite
    // That is - before other tests from this class run
}

void UnitTests::cleanupTestCase() {
    // Similarly to initTestCase(), this function is executed at the end of test suite
}

void UnitTests::init() {
    // This function is executed before each test
}

void UnitTests::cleanup() {
    // This function is executed after each test
}

void UnitTests::testSavingAndLoadingFile() {
    MainWindow mainWindow;

    QString filename("output.txt");

    // First, remove output file, if it exists
    QFile outputFile(filename);
    if (outputFile.exists()) {
        if (!outputFile.remove()) {
            QFAIL("Could not remove file");
        }
    }

    // Save the current document
    mainWindow.SaveJsonDocumentToDisk();

    // Load the file saved in previous step into a QJsonDocument
    QJsonDocument doc;
    int errorCode;
    JsonFileIO::LoadJsonDocumentFromDisk(doc, filename, errorCode);
    if (errorCode != JsonFileIOError::NoError) {
        QFAIL("Error loading JSON document from disk");
    }

    // Verify that the file has the FruitA root level object
    QJsonValue jsonValue = doc.object().take("FruitA");
    QVERIFY2(jsonValue.toString() == "Apple", "FruitA should be Apple");
}
