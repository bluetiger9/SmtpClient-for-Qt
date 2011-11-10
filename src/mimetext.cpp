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

#include "mimetext.h"

/* [1] Constructors and Destructors */

MimeText::MimeText(const QString &text)
{
    this->text = text;
    this->charset = "utf-8";
    this->encoding = _7Bit;
}

MimeText::~MimeText() { }

/* [1] --- */


/* [2] Getters and Setters */

void MimeText::setText(const QString & text)
{
    this->text = text;
}

void MimeText::setEncoding(MimePart::Encoding enc)
{
    this->encoding = enc;
}

void MimeText::setCharset(const QString & charset)
{
    this->charset = charset;
}

const QString & MimeText::getText() const
{
    return text;
}

MimePart::Encoding MimeText::getEncoding() const
{
    return encoding;
}

const QString & MimeText::getCharset() const
{
    return charset;
}

/* [2] --- */


/* [3] Protected Methods */

void MimeText::prepare()
{
    this->header = "Content-Type: text/plain; charset="
            + this->charset + "\n";

    this->header += "Content-Transfer-Encoding: ";

    switch (encoding)
    {
    case _7Bit:
        header += "7bit\n";
        content = this->text.toAscii();
        break;
    case _8Bit:
        header += "8bit\n";
        content = this->text.toUtf8();
        break;
    case Base64:
        header += "base64\n";
        content = QByteArray().append(this->text).toBase64();
    }

    this->content += "\n";

}

/* [3] --- */
