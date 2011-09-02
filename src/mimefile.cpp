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
    this->type = "application/octet-stream";
    this->name = QFileInfo(*file).fileName();
}

MimeFile::~MimeFile()
{
}

/* [1] --- */


/* [2] Getters and setters */

void MimeFile::setName(const QString & name)
{
    this->name = name;
}

void MimeFile::setType(const QString & type)
{
    this->type = type;
}

const QString & MimeFile::getName() const
{
    return name;
}

const QString & MimeFile::getType() const
{
    return type;
}

/* [2] --- */


/* [3] Protected methods */

void MimeFile::prepare()
{
    this->header = "Content-Type: " + type + "; name=" + name + "\n";
    this->header += "Content-Transfer-Encoding: base64\n";

    file->open(QIODevice::ReadOnly);
    this->content = file->readAll().toBase64() + "\n";
    file->close();
}

/* [3] --- */

