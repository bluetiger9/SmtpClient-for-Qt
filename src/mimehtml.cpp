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

#include "mimehtml.h"

/* [1] Constructors and Destructors */

MimeHtml::MimeHtml(const QString &html)
{
    this->html = html;
    this->encoding = _7Bit;
    this->charset = "utf-8";
}

MimeHtml::~MimeHtml() {}

/* [1] --- */


/* [2] Getters and Setters */

void MimeHtml::setHtml(const QString & html)
{
    this->html = html;
}

void MimeHtml::setEncoding(MimePart::Encoding enc)
{
    this->encoding = enc;
}

void MimeHtml::setCharset(const QString & charset)
{
    this->charset = charset;
}

const QString & MimeHtml::getHtml() const
{
    return html;
}

const QString & MimeHtml::getCharset() const
{
    return charset;
}

MimePart::Encoding MimeHtml::getEncoding() const
{
    return encoding;
}

/* [2] --- */


/* [3] Protected methods */

void MimeHtml::prepare()
{
    this->header = "Content-Type: text/html; charset="
            + this->charset + "\n";

    this->header += "Content-Transfer-Encoding: ";

    switch (encoding)
    {
    case _7Bit:
        header += "7bit\n";
        content = this->html.toAscii();
        break;
    case _8Bit:
        header += "8bit\n";
        content = this->html.toUtf8();
        break;
    case Base64:
        header += "base64\n";
        content = QByteArray().append(this->html).toBase64();
    }

    this->content += "\n";
}

/* [3] --- */
