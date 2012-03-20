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

#ifndef MIMEATTACHMENT_H
#define MIMEATTACHMENT_H

#include <QFile>
#include "mimepart.h"
#include "mimefile.h"

class MimeAttachment : public MimeFile
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    MimeAttachment(QFile* file);
    ~MimeAttachment();

    /* [1] --- */

protected:

    /* [2] Protected methods */

    virtual void prepare();

    /* [2] --- */
};

#endif // MIMEATTACHMENT_H
