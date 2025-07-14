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
#include "mimemultipart.h"
#include <typeinfo>

/* [1] Constructors and Destructors */

MimeMessage::MimeMessage(bool createAutoMimeContent) :
    hEncoding(MimePart::_8Bit)
{
    this->mimeContentAutoCreated = createAutoMimeContent;
    if (createAutoMimeContent) {
        this->content = new MimeMultiPart();
    }
}

MimeMessage::~MimeMessage()
{
    if (this->mimeContentAutoCreated) {
        delete this->content;
    }
}

/* [1] --- */


/* [2] Getters and Setters */
MimePart& MimeMessage::getContent() {
    return *content;
}

void MimeMessage::setContent(MimePart *content) {
    this->content = content;
}

void MimeMessage::setSender(const EmailAddress &sender)
{
    this->sender = sender;
}

void MimeMessage::setReplyTo(const EmailAddress &replyTo)
{
    this->replyTo = replyTo;
}

void MimeMessage::addRecipient(const EmailAddress &rcpt, RecipientType type)
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

void MimeMessage::addTo(const EmailAddress &rcpt) {
    this->recipientsTo << rcpt;
}

void MimeMessage::addCc(const EmailAddress &rcpt) {
    this->recipientsCc << rcpt;
}

void MimeMessage::addBcc(const EmailAddress &rcpt) {
    this->recipientsBcc << rcpt;
}

void MimeMessage::addCustomHeader(const QString &header)
{
    this->customHeaders << header;
}

void MimeMessage::setSubject(const QString & subject)
{
    this->subject = subject;
}

void MimeMessage::addPart(MimePart *part) {
    this->addPart(part, false);
}

void MimeMessage::addPart(MimePart *part, const bool takeOwnership)
{
    if (typeid(*content) == typeid(MimeMultiPart)) {
        ((MimeMultiPart*) content)->addPart(part, takeOwnership);
    };
}

void MimeMessage::setHeaderEncoding(MimePart::Encoding hEnc)
{
    this->hEncoding = hEnc;
}

EmailAddress MimeMessage::getSender() const
{
    return sender;
}

EmailAddress MimeMessage::getReplyTo() const
{
    return replyTo;
}

const QList<EmailAddress> & MimeMessage::getRecipients(RecipientType type) const
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

QString MimeMessage::getSubject() const
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

QString MimeMessage::toString() const
{
    QBuffer out;
    out.open(QIODevice::WriteOnly);
    writeToDevice(out);
    return QString(out.buffer());
}

QByteArray MimeMessage::formatAddress(const EmailAddress &address, MimePart::Encoding encoding) {
    QByteArray result;
    result.append(format(address.getName(), encoding));
    result.append((" <" + address.getAddress() + ">").toUtf8());
    return result;
}

QByteArray MimeMessage::format(const QString &text, MimePart::Encoding encoding)
{
    QByteArray result;
    if (!text.isEmpty())
    {
        switch (encoding)
        {
        case MimePart::Base64:
            result.append(" =?utf-8?B?" + text.toUtf8().toBase64() + "?=");
            break;
        case MimePart::QuotedPrintable:
            result.append(" =?utf-8?Q?" + QuotedPrintable::encode(text.toUtf8()).toLocal8Bit().replace(' ', "_").replace(':',"=3A").replace(",", "=2C") + "?=");
            break;
        default:
            result.append(" ").append(text.toLocal8Bit());
        }
    }
    return result;
}

void MimeMessage::writeToDevice(QIODevice &out) const {
    /* =========== MIME HEADER ============ */

    /* ---------- Sender / From ----------- */
    QByteArray header;
    header.append("From:" + formatAddress(sender, hEncoding) + "\r\n");
    /* ---------------------------------- */

    /* ---------- Reply-To ----------- */
    if (!replyTo.getAddress().isEmpty()) {
      header.append("Reply-To:" + formatAddress(replyTo, hEncoding) + "\r\n");
    }
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
    header.append(format(subject, hEncoding));
    header.append("\r\n");
    /* ---------------------------------- */

    foreach (QString hdr, customHeaders) {
        header.append(hdr.toLocal8Bit());
        header.append("\r\n");
    }

    header.append("MIME-Version: 1.0\r\n");

    out.write(header);
    content->writeToDevice(out);
}

/* [3] --- */
