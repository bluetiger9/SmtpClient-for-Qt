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

MimeHtml::MimeHtml(const QString &html) : MimeText(html)
{
    this->cType = "text/html";
}

MimeHtml::~MimeHtml() {}

/* [1] --- */


/* [2] Getters and Setters */

void MimeHtml::setHtml(const QString & html)
{
    this->text = html;
}

const QString & MimeHtml::getHtml() const
{
    return text;
}


/* [2] --- */


/* [3] Protected methods */

void MimeHtml::prepare()
{
    /* !!! IMPORTANT !!! */
    MimeText::prepare();
}

/* [3] --- */
