#include <QtCore>

#include "../../src/SmtpMime"
#include "../demo_vars.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // This is a first demo application of the SmtpClient for Qt project

    // Now we create a MimeMessage object. This is the email.

    MimeMessage message;

    EmailAddress sender(SENDER_EMAIL, SENDER_NAME);
    message.setSender(sender);

    EmailAddress to(RECIPIENT_EMAIL, RECIPIENT_NAME);
    message.addRecipient(to);

    message.setSubject("SmtpClient for Qt - Demo");

    // Now add some text to the email.
    // First we create a MimeText object.

    MimeText text;

    text.setText("Hi,\nThis is a simple email message.\n");

    // Now add it to the mail

    message.addPart(&text);

    // Now we can send the mail
    SmtpClient smtp(SMTP_SERVER, 465, SmtpClient::SslConnection);

    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) {
        qDebug() << "Failed to connect to host!";
        return -1;
    }

    smtp.login(SENDER_EMAIL, SENDER_PASSWORD);
    if (!smtp.waitForAuthenticated()) {
        qDebug() << "Failed to login!";
        return -2;
    }

    smtp.sendMail(message);
    if (!smtp.waitForMailSent()) {
        qDebug() << "Failed to send mail!";
        return -3;
    }

    smtp.quit();

}
