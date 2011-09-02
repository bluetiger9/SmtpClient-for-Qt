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

#include "mimeinlinefile.h"

/* [1] Constructors and Destructors */

MimeInlineFile::MimeInlineFile(QFile *f)
    : MimeFile(f)
{
    return;
}

MimeInlineFile::~MimeInlineFile()
{}

/* [1] --- */


/* [2] Getters and Setters */

void MimeInlineFile::setContentId(const QString & cid)
{
    this->cid = cid;
}

const QString & MimeInlineFile::getContentId() const
{
    return cid;
}

/* [2] --- */


/* [3] Protected methods */

void MimeInlineFile::prepare()
{
    MimeFile::prepare();

    this->header += "Content-id: <" + cid + ">\n";
    this->header += "Content-disposition: inline\n";
}

/* [3] --- */



