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

#include "mimetext.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringEncoder>
#else
#include <QTextEncoder>
#endif

/* [1] Constructors and Destructors */

MimeText::MimeText(const QString &txt)
{
    this->text = txt;
    this->cType = "text/plain";
    this->cCharset = "utf-8";
    this->cEncoding = _8Bit;
}

MimeText::~MimeText() { }

/* [1] --- */


/* [2] Getters and Setters */

void MimeText::setText(const QString & text)
{
    this->text = text;
}

const QString & MimeText::getText() const
{
    return text;
}

/* [2] --- */


/* [3] Protected Methods */

void MimeText::writeContent(QIODevice &device) const {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    MimePart::writeContent(device, QStringEncoder(this->cCharset.toStdString().c_str()).encode(text));
#else
    MimePart::writeContent(device, QTextEncoder(QTextCodec::codecForName(this->cCharset.toStdString().c_str())).fromUnicode(text));
#endif
}

/* [3] --- */
