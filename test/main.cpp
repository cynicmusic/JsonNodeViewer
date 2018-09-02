#include <QtTest>
#include "unittests.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UnitTests unitTests;

    return QTest::qExec(&unitTests, argc, argv);
}
