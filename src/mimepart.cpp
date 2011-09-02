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

#include "mimepart.h"

/* [1] Constructors and Destructors */

MimePart::MimePart()
{
}

MimePart::~MimePart()
{
    return;
}

/* [1] --- */


/* [2] Getters and Setters */

void MimePart::setContent(const QString & content)
{
    this->content = content;
}

void MimePart::setHeader(const QString & header)
{
    this->header = header;
}

void MimePart::addHeaderLine(const QString & line)
{
    this->header += line;
}

const QString& MimePart::getHeader() const
{
    return header;
}

const QString& MimePart::getContent() const
{
    return content;
}

/* [2] --- */


/* [3] Public methods */

QString MimePart::toString()
{
    prepare();
    return header + "\n" + content;
}

/* [3] --- */


/* [4] Protected methods */

void MimePart::prepare() {}

/* [4] --- */
