#include <QtCore>

#include "../../src/SmtpMime"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // This is a first demo application of the SmtpClient for Qt project

    // Now we create a MimeMessage object. This is the email.

    MimeMessage message;

    EmailAddress sender("your_email_address@host.com", "Your Name");
    message.setSender(sender);

    EmailAddress to("recipient@host.com", "Recipient's Name");
    message.addRecipient(to);

    message.setSubject("SmtpClient for Qt - Demo");

    // Now add some text to the email.
    // First we create a MimeText object.

    MimeText text;

    text.setText("Hi,\nThis is a simple email message.\n");

    // Now add it to the mail

    message.addPart(&text);

    // Now we can send the mail
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) {
        qDebug() << "Failed to connect to host!" << endl;
        return -1;
    }

    smtp.login("your_email_address@host.com", "your_password");
    if (!smtp.waitForAuthenticated()) {
        qDebug() << "Failed to login!" << endl;
        return -2;
    }

    smtp.sendMail(message);
    if (!smtp.waitForMailSent()) {
        qDebug() << "Failed to send mail!" << endl;
        return -3;
    }

    smtp.quit();

}
