#include "connectiontest.h"
#include <QDebug>
#include <QtTest/QtTest>
#include "../src/smtpclient.h"

ConnectionTest::ConnectionTest(QObject *parent) :
    QObject(parent) {}


void ConnectionTest::init() {
    //qDebug() << "Init...";
}

void ConnectionTest::testConnect() {
    QFETCH(QString, host);
    QFETCH(int, port);
    QFETCH(int, connectionType);

    const SmtpClient::ConnectionType cTypes[] = {
        SmtpClient::TcpConnection,
        SmtpClient::SslConnection,
        SmtpClient::TcpConnection
    };

    SmtpClient::ConnectionType cType = cTypes[--connectionType];

    SmtpClient smtp(host, port, cType);
    smtp.connectToHost();

    QCOMPARE(smtp.waitForReadyConnected(5000), true);
}

void ConnectionTest::testConnect_data() {
    QTest::addColumn<QString>("host");
    QTest::addColumn<int>("port");
    QTest::addColumn<int>("connectionType");

    QFile file("../connect_data.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString host;
        int port;
        int connectionType;
        in >> host >> port >> connectionType;
        if (!host.isEmpty()) {
            QTest::newRow(QString("%1:%2").arg(host).arg(port).toLocal8Bit().data()) << host << port << connectionType;
        }
    }
    file.close();
}

void ConnectionTest::cleanup() {
}
