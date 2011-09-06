#include <QtGui/QApplication>
#include "../src/SmtpMime"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // First create the SmtpClient object and set the user and the password.

    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    smtp.setUser("your_email_address@gmail.com");
    smtp.setPassword("your_password");

    // Create a MimeMessage

    MimeMessage message;

    message.setSender(new EmailAddress("your_email_address@gmail.com", "Your Name"));
    message.addRecipient(new EmailAddress("recipient@host.com", "Recipient's Name"));
    message.setSubject("SmtpClient for Qt - Demo");

    // Add some text

    MimeText text;

    text.setText("Hi,\nThis is a simple email message with some attachments.\n");

    message.addPart(&text);


    // Now we create the attachment object

    MimeAttachment attachment (new QFile("image1.jpg"));

    // the file type can be setted. (by default is application/octet-stream)

    attachment.setType("image/jpg");

    // Now add it to message

    message.addPart(&attachment);

    // Add an another attachment
    message.addPart(new MimeAttachment(new QFile("document.pdf")));

    // Now we can send the mail

    smtp.connectToHost();
    smtp.login();
    smtp.sendMail(message);
    smtp.quit();

}
