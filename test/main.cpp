#include <QCoreApplication>
#include <QtTest/QTest>
#include <QDebug>
#include "connectiontest.h"

bool success = true;

static void runTest(QObject *test, int argc, char** argv) {
    int retVal = QTest::qExec(test, argc, argv);
    delete test;
    success &= retVal == 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    runTest(new ConnectionTest(), argc, argv);

    if (success)
        qDebug() << "SUCCESS";
    else
        qDebug() << "FAIL";

    return success;
}
