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

#include <QtGui/QApplication>
#include "../src/SmtpMime"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // First create the SmtpClient object and set the user and the password.

    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    smtp.setUser("your_email@gmail.com");
    smtp.setPassword("your_password");

    // Create a MimeMessage

    MimeMessage message;

    message.setSender(new EmailAddress("your_email@gmail.com", "Your Name"));
    message.addRecipient(new EmailAddress("recipient@host.com", "Recipient's Name"));
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
    message.addPart(new MimeAttachment(new QFile("document.pdf")));

    // Now we can send the mail

    smtp.connectToHost();
    smtp.login();
    smtp.sendMail(message);
    smtp.quit();

}
