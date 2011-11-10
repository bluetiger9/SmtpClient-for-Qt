#include <QtGui/QApplication>
#include "src/SmtpMime"
#include <iostream>

int main(int argc, char* args[])
{
    QApplication a(argc, args);

    MimeMessage message;

    //MimeText text("A nevem Tőkés Attila");
    //text.setCharset("utf-8");
    //text.setEncoding(MimePart::Base64);


    message.setSender(new EmailAddress("attitokes@gmail.com", "Tőkés 1 Attilaîăăâîă"));
    message.addRecipient(new EmailAddress("attitokes@gmail.com", "Tőksasdasdés Attiladfdfdfdf"));
    message.setSubject("Ékezetek tesztje: áúúőűúőűúőűúőöüóöüóöüöŐÚÉÁÉÚÉÁÉÁ");

    message.addPart(&text);
    message.useBase64InHeaders(true);

    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);
    smtp.setUser("attitokes@gmail.com");
    smtp.setPassword("miniflash10b");

    std::cout << smtp.connectToHost();
    std::cout << smtp.login();
    std::cout << smtp.sendMail(message);
    smtp.quit();

    //QString s = MimeTools::quotedPrintable("A nevem Tőkés Attila");
    //std::cout << MimeTools::quotedPrintable("A nevem Tőkés Attila").toStdString();

    return 0;
}
