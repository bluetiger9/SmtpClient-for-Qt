/*
  Copyright (c) 2011-2012 - Tőkés Attila

  This file is part of SmtpClient for Qt.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  See the LICENSE file for more details.
*/

#include "mimemessage.h"

#include <QDebug>
#include <QDateTime>
#include <QBuffer>
#include "quotedprintable.h"
#include <typeinfo>

/* [1] Constructors and Destructors */

MimeMessage::MimeMessage(bool createAutoMimeContent) :
    hEncoding(MimePart::_8Bit)
{
    if (createAutoMimeContent)
        this->content = new MimeMultiPart();
}

MimeMessage::~MimeMessage()
{
}

/* [1] --- */


/* [2] Getters and Setters */
MimePart& MimeMessage::getContent() {
    return *content;
}

void MimeMessage::setContent(MimePart *content) {
    this->content = content;
}

void MimeMessage::setSender(EmailAddress* e)
{
    this->sender = e;
}

void MimeMessage::addRecipient(EmailAddress* rcpt, RecipientType type)
{
    switch (type)
    {
    case To:
        recipientsTo << rcpt;
        break;
    case Cc:
        recipientsCc << rcpt;
        break;
    case Bcc:
        recipientsBcc << rcpt;
        break;
    }
}

void MimeMessage::addTo(EmailAddress* rcpt) {
    this->recipientsTo << rcpt;
}

void MimeMessage::addCc(EmailAddress* rcpt) {
    this->recipientsCc << rcpt;
}

void MimeMessage::addBcc(EmailAddress* rcpt) {
    this->recipientsBcc << rcpt;
}

void MimeMessage::setSubject(const QString & subject)
{
    this->subject = subject;
}

void MimeMessage::addPart(MimePart *part)
{
    if (typeid(*content) == typeid(MimeMultiPart)) {
        ((MimeMultiPart*) content)->addPart(part);
    };
}

void MimeMessage::setHeaderEncoding(MimePart::Encoding hEnc)
{
    this->hEncoding = hEnc;
}

const EmailAddress & MimeMessage::getSender() const
{
    return *sender;
}

const QList<EmailAddress*> & MimeMessage::getRecipients(RecipientType type) const
{
    switch (type)
    {
    default:
    case To:
        return recipientsTo;
    case Cc:
        return recipientsCc;
    case Bcc:
        return recipientsBcc;
    }
}

const QString & MimeMessage::getSubject() const
{
    return subject;
}

const QList<MimePart*> & MimeMessage::getParts() const
{
    if (typeid(*content) == typeid(MimeMultiPart)) {
        return ((MimeMultiPart*) content)->getParts();
    }
    else {
        QList<MimePart*> *res = new QList<MimePart*>();
        res->append(content);
        return *res;
    }
}

/* [2] --- */


/* [3] Public Methods */

QString MimeMessage::toString()
{
    QBuffer out;
    out.open(QIODevice::WriteOnly);
    writeToDevice(out);
    return QString(out.buffer());
}

QByteArray MimeMessage::formatAddress(EmailAddress *address, MimePart::Encoding encoding) {
    QByteArray result;
    if (!address->getName().isEmpty())
    {
        switch (encoding)
        {
        case MimePart::Base64:
            result.append(" =?utf-8?B?" + address->getName().toLocal8Bit().toBase64() + "?=");
            break;
        case MimePart::QuotedPrintable:
            result.append(" =?utf-8?Q?" + QuotedPrintable::encode(address->getName().toLocal8Bit()).toLocal8Bit().replace(' ', "_").replace(':',"=3A") + "?=");
            break;
        default:
            result.append(" ").append(address->getName().toLocal8Bit());
        }
    }
    result.append(" <" + address->getAddress() + ">");
    return result;
}

void MimeMessage::writeToDevice(QIODevice &out) {
    /* =========== MIME HEADER ============ */

    /* ---------- Sender / From ----------- */
    QByteArray header;
    header.append("From:" + formatAddress(sender, hEncoding) + "\r\n");
    /* ---------------------------------- */

    /* ------- Recipients / To ---------- */
    header.append("To:");
    for (int i = 0; i<recipientsTo.size(); ++i)
    {
        if (i != 0) { header.append(","); }
        header.append(formatAddress(recipientsTo.at(i), hEncoding));
    }
    header.append("\r\n");
    /* ---------------------------------- */

    /* ------- Recipients / Cc ---------- */
    if (recipientsCc.size() != 0) {
        header.append("Cc:");
    }
    for (int i = 0; i<recipientsCc.size(); ++i)
    {
        if (i != 0) { header.append(","); }
        header.append(formatAddress(recipientsCc.at(i), hEncoding));
    }
    if (recipientsCc.size() != 0) {
        header.append("\r\n");
    }
    /* ---------------------------------- */

    /* ------------ Subject ------------- */
    header.append("Subject: ");


    switch (hEncoding)
    {
    case MimePart::Base64:
        header.append("=?utf-8?B?" + subject.toLocal8Bit().toBase64() + "?=");
        break;
    case MimePart::QuotedPrintable:
        header.append("=?utf-8?Q?" + QuotedPrintable::encode(subject.toLocal8Bit()).toLocal8Bit().replace(' ', "_").replace(':',"=3A") + "?=");
        break;
    default:
        header.append(subject);
    }
    /* ---------------------------------- */

    header.append("\r\n");
    header.append("MIME-Version: 1.0\r\n");

    out.write(header);
    content->writeToDevice(out);
}

/* [3] --- */
