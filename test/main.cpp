#include <QtGui/QApplication>
#include <QCoreApplication>
#include <QtTest/QTest>
#include <QDebug>
#include "connectiontest.h"

bool success = true;

void runTest(QObject *test) {
    int retVal = QTest::qExec(test);
    success &= retVal == 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    runTest(new ConnectionTest());

    if (success)
        qDebug() << "SUCCESS";
    else
        qDebug() << "FAIL";

    return success;
}
