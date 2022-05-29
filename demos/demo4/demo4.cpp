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
#include "../demo_vars.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create a MimeMessage

    MimeMessage message;

    EmailAddress sender(SENDER_EMAIL, SENDER_NAME);
    message.setSender(sender);

    EmailAddress to(RECIPIENT_EMAIL, RECIPIENT_NAME);
    message.addRecipient(to);

    message.setSubject("SmtpClient for Qt - Example 3 - Html email with images");

    // Now we need to create a MimeHtml object for HTML content
    MimeHtml html;

    html.setHtml("<h1> Hello! </h1>"
                 "<h2> This is the first image </h2>"
                 "<img src='cid:image1' />"
                 "<h2> This is the second image </h2>"
                 "<img src='cid:image2' />");


    // Create a MimeInlineFile object for each image
    QFile imageFile1("image1.jpg");
    MimeInlineFile image1(&imageFile1);

    // An unique content id must be setted
    image1.setContentId("image1");
    image1.setContentType("image/jpg");

    QFile imageFile2("image2.jpg");
    MimeInlineFile image2(&imageFile2);

    image2.setContentId("image2");
    image2.setContentType("image/jpg");

    message.addPart(&html);
    message.addPart(&image1);
    message.addPart(&image2);

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
