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

#include "mimefile.h"

#include <QFileInfo>

/* [1] Constructors and Destructors */

MimeFile::MimeFile(QFile *file)
{
    this->file = file;
    this->cType = "application/octet-stream";
    this->cName = QFileInfo(*file).fileName();
    this->cEncoding = Base64;
}

MimeFile::~MimeFile()
{
}

/* [1] --- */


/* [2] Getters and setters */

/* [2] --- */


/* [3] Protected methods */

void MimeFile::prepare()
{
    file->open(QIODevice::ReadOnly);
    this->content = file->readAll();
    file->close();

    /* !!! IMPORTANT !!!! */
    MimePart::prepare();
}

/* [3] --- */

