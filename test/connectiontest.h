#ifndef CONNECTIONTEST_H
#define CONNECTIONTEST_H

#include <QObject>

class ConnectionTest : public QObject
{
    Q_OBJECT
public:
    ConnectionTest(QObject *parent = 0);
    
private slots:

    void init();
    void cleanup();

    void testConnect();
    void testConnect_data();
};

#endif // CONNECTIONTEST_H
