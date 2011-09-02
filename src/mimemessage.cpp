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

#include "mimemessage.h"

#include <QDateTime>

/* [1] Constructors and Destructors */

MimeMessage::MimeMessage()
{
}

MimeMessage::~MimeMessage()
{

}

/* [1] --- */


/* [2] Getters and Setters */

void MimeMessage::setSender(EmailAddress* e)
{
    this->sender = e;
}

void MimeMessage::addRecipient(EmailAddress* rcpt)
{
    this->recipients << rcpt;
}

void MimeMessage::setSubject(const QString & subject)
{
    this->subject = subject;
}

void MimeMessage::addPart(MimePart *part)
{
    this->parts << part;
}

const EmailAddress & MimeMessage::getSender() const
{
    return *sender;
}

const QList<EmailAddress*> & MimeMessage::getRecipients() const
{
    return recipients;
}

const QString & MimeMessage::getSubject() const
{
    return subject;
}

const QList<MimePart*> & MimeMessage::getParts() const
{
    return parts;
}

/* [2] --- */


/* [3] Public Methods */

QString MimeMessage::toString()
{
    QString mime;
    mime = "From: " + sender->getName() + " <" + sender->getAddress() + ">\n";

    QList<EmailAddress*>::iterator it;
    for (it = recipients.begin(); it != recipients.end(); ++it)
        mime += "To: " + (*it)->getName() + " <" + (*it)->getAddress() + ">\n";

    mime += "Subject: " + subject + "\n";

    QString boundary = "----MIME-part-boundary=" + QByteArray().append(QDateTime::currentDateTime().toString()).toBase64() + "-end";

    mime += "MIME-Version: 1.0\n";
    mime += "Content-type: multipart/mixed; boundary=\"" + boundary + "\"\n\n";

    boundary = "--" + boundary;

    QList<MimePart*>::iterator i;
    for (i = parts.begin(); i != parts.end(); ++i)
        mime += boundary + "\n" + (*i)->toString();

    mime += boundary + "--\n";

    return mime;
}

/* [3] --- */
