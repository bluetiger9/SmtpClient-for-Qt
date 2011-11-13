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

void MimeText::prepare()
{
    this->content.clear();
    this->content.append(text);

    /* !!! IMPORTANT !!! */
    MimePart::prepare();
}

/* [3] --- */
