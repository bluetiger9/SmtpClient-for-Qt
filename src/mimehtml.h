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

#ifndef MIMEHTML_H
#define MIMEHTML_H

#include "mimepart.h"

class MimeHtml : public MimePart
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    MimeHtml(const QString &html = "");
    ~MimeHtml();

    /* [1] --- */


    /* [2] Getters and Setters */

    void setHtml(const QString & html);
    void setEncoding(Encoding enc);
    void setCharset(const QString & charset);

    const QString& getHtml() const;
    const QString& getCharset() const;
    Encoding getEncoding() const;

    /* [2] --- */

protected:

    /* [3] Protected members */

    QString html;
    QString charset;
    Encoding encoding;

    /* [3] --- */


    /* [4] Protected methods */

    void prepare();

    /* [4] --- */
};

#endif // MIMEHTML_H
