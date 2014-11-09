/*
  Copyright (c) 2011 - Tőkés Attila

  This file is part of SmtpClient for Qt.

  SmtpClient for Qt is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  SmtpClient for Qt is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY.

  See the LICENSE file for more details.
*/

#include <QtCore>

#include "../../src/SmtpMime"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create a MimeMessage
    MimeMessage message;

    EmailAddress sender("your_email_address@host.com", "Your Name");
    message.setSender(sender);

    EmailAddress to("recipient@host.com", "Recipient's Name");
    message.addRecipient(to);

    message.setSubject("SmtpClient for Qt - Demo");

    // Add some text
    MimeText text;
    text.setText("Hi!\n This is an email with some attachments.");
    message.addPart(&text);

    // Now we create the attachment object
    MimeAttachment attachment (new QFile("image1.jpg"));

    // the file type can be setted. (by default is application/octet-stream)
    attachment.setContentType("image/jpg");

    // Now add it to message
    message.addPart(&attachment);

    // Add an another attachment
    MimeAttachment document(new QFile("document.pdf"));
    message.addPart(&document);

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
