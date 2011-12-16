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

#include "mimeattachment.h"
#include <QFileInfo>

/* [1] Constructors and Destructors */

MimeAttachment::MimeAttachment(QFile *file)
    : MimeFile(file)
{
}

MimeAttachment::~MimeAttachment()
{
}

/* [1] --- */


/* [2] Protected methods */

void MimeAttachment::prepare()
{
    this->header += "Content-disposition: attachment\r\n";

    /* !!! IMPORTANT !!! */
    MimeFile::prepare();
}

/* [2] --- */
