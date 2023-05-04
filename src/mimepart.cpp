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

#include <QBuffer>
#include "mimepart.h"
#include "quotedprintable.h"
#include "mimebase64formatter.h"
#include "mimeqpformatter.h"
#include "mimebase64encoder.h"
#include "mimeqpencoder.h"

/* [1] Constructors and Destructors */

MimePart::MimePart()
{
    cEncoding = _7Bit;
    prepared = false;
    cBoundary = "";
}

MimePart::~MimePart()
{
    return;
}

/* [1] --- */


/* [2] Getters and Setters */

void MimePart::setContent(const QByteArray & content)
{
    this->content = content;
}

void MimePart::setHeader(const QString & header)
{
    this->headerLines = header;
}

void MimePart::addHeaderLine(const QString & line)
{
    this->headerLines += line + "\r\n";
}

QString MimePart::getHeader() const
{
    return headerLines;
}

QByteArray MimePart::getContent() const
{
    return content;
}

void MimePart::setContentId(const QString & cId)
{
    this->cId = cId;
}

QString MimePart::getContentId() const
{
    return this->cId;
}

void MimePart::setContentName(const QString & cName)
{
    this->cName = cName;
}

QString MimePart::getContentName() const
{
    return this->cName;
}

void MimePart::setContentType(const QString & cType)
{
    this->cType = cType;
}

QString MimePart::getContentType() const
{
    return this->cType;
}

void MimePart::setCharset(const QString & charset)
{
    this->cCharset = charset;
}

QString MimePart::getCharset() const
{
    return this->cCharset;
}

void MimePart::setEncoding(Encoding enc)
{
    this->cEncoding = enc;
}

MimePart::Encoding MimePart::getEncoding() const
{
    return this->cEncoding;
}

void MimePart::setMaxLineLength(const int length) {
    maxLineLength = length;
}

int MimePart::getMaxLineLength() const {
    return maxLineLength;
}

/* [2] --- */


/* [3] Public methods */

QString MimePart::toString() const
{
    QBuffer out;
    out.open(QIODevice::WriteOnly);
    writeToDevice(out);
    return QString(out.buffer());
}

void MimePart::writeToDevice(QIODevice &device) const {
    QString header;

    /* === Header Prepare === */

    /* Content-Type */
    header.append("Content-Type: ").append(cType);

    if (cName != "")
        header.append("; name=\"").append(cName).append("\"");

    if (cCharset != "")
        header.append("; charset=").append(cCharset);

    if (cBoundary != "")
        header.append("; boundary=").append(cBoundary);

    header.append("\r\n");
    /* ------------ */

    /* Content-Transfer-Encoding */
    header.append("Content-Transfer-Encoding: ");
    switch (cEncoding)
    {
    case _7Bit:
        header.append("7bit\r\n");
        break;
    case _8Bit:
        header.append("8bit\r\n");
        break;
    case Base64:
        header.append("base64\r\n");
        break;
    case QuotedPrintable:
        header.append("quoted-printable\r\n");
        break;
    }
    /* ------------------------ */

    /* Content-Id */
    if (!cId.isEmpty()) {
        header.append("Content-ID: <").append(cId).append(">\r\n");
    }
    /* ---------- */

    /* Additional header lines */

    header.append(headerLines).append("\r\n");

    /* ------------------------- */

    /* === End of Header Prepare === */

    device.write(header.toUtf8());

    writeContent(device);
}



/* [3] --- */


/* [4] Protected methods */

void MimePart::writeContent(QIODevice &device) const {
    this->writeContent(device, content);
}

void MimePart::writeContent(QIODevice &device, const QByteArray &content) const {
    switch (cEncoding)
    {
    case _7Bit:
    case _8Bit:
        device.write(content);
        break;
    case Base64:
        MimeBase64Formatter(&device).write(MimeBase64Encoder().encode(content));
        break;
    case QuotedPrintable:
        MimeQPFormatter(&device).write(MimeQpEncoder().encode(content));
        break;
    }
    device.write("\r\n");
}

/* [4] --- */
