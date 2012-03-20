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
#include "quotedprintable.h"

/* [1] Constructors and Destructors */

MimeMessage::MimeMessage() :
    hEncoding(MimePart::_8Bit)
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

void MimeMessage::setHeaderEncoding(MimePart::Encoding hEnc)
{
    this->hEncoding = hEnc;
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

    /* =========== MIME HEADER ============ */

    /* ---------- Sender / From ----------- */
    mime = "From:";
    if (sender->getName() != "")
    {
        switch (hEncoding)
        {
        case MimePart::Base64:
            mime += " =?utf-8?B?" + QByteArray().append(sender->getName()).toBase64() + "?=";
            break;
        case MimePart::QuotedPrintable:
            mime += " =?utf-8?Q?" + QuotedPrintable::encode(QByteArray().append(sender->getName())).replace(' ', "_").replace(':',"=3A") + "?=";
            break;
        default:
            mime += " " + sender->getName();
        }
    }
    mime += " <" + sender->getAddress() + ">\r\n";
    /* ---------------------------------- */


    /* ------- Recipients / To ---------- */
    QList<EmailAddress*>::iterator it;
    for (it = recipients.begin(); it != recipients.end(); ++it)
    {
        mime += "To:";

        if ((*it)->getName() != "")
        {
            switch (hEncoding)
            {
            case MimePart::Base64:
                mime += " =?utf-8?B?" + QByteArray().append((*it)->getName()).toBase64() + "?=";
                break;
            case MimePart::QuotedPrintable:
                mime += " =?utf-8?Q?" + QuotedPrintable::encode(QByteArray().append((*it)->getName())).replace(' ', "_").replace(':',"=3A") + "?=";
                break;
            default:
                mime += " " + (*it)->getName();
            }
        }
        mime += " <" + (*it)->getAddress() + ">\r\n";
    }
    /* ---------------------------------- */

    /* ------------ Subject ------------- */
    mime += "Subject: ";

    switch (hEncoding)
    {
    case MimePart::Base64:
        mime += "=?utf-8?B?" + QByteArray().append(subject).toBase64() + "?=";
        break;
    case MimePart::QuotedPrintable:
        mime += "=?utf-8?Q?" + QuotedPrintable::encode(QByteArray().append(subject)).replace(' ', "_").replace(':',"=3A") + "?=";
        break;
    default:
        mime += subject;
    }
    /* ---------------------------------- */

    mime += "\r\n";

    QString boundary = "----MIME-part-boundary=" + QByteArray().append(QDateTime::currentDateTime().toString()).toBase64() + "-end";

    mime += "MIME-Version: 1.0\r\n";
    mime += "Content-type: multipart/mixed; boundary=\"" + boundary + "\"\r\n\r\n";

    /* ====== END OF MIME HEADER ======== */

    boundary = "--" + boundary;

    /* ========== MIME BODY ============= */

    QList<MimePart*>::iterator i;
    for (i = parts.begin(); i != parts.end(); ++i)
        mime += boundary + "\r\n" + (*i)->toString();

    mime += boundary + "--\r\n";

    /* ====== END OF MIME BODY ========= */

    return mime;
}

/* [3] --- */
