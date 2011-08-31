#include <QtGui/QApplication>

#include "sendemail.h"
#include "../../src/SmtpMime"


#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SendEmail form (NULL);

    form.setWindowTitle("SmtpClient for Qt - Demo 2");

    form.show();

    a.exec();


}

