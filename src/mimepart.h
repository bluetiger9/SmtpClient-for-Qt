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

#ifndef MIMEPART_H
#define MIMEPART_H

#include <QObject>

class MimePart : public QObject
{
    Q_OBJECT
public:

    /* [0] Enumerations */
    enum Encoding {        
        _7Bit,
        _8Bit,
        Base64,
        QuotedPrintable
    };


    /* [0] --- */


    /* [1] Constructors and Destructors */

    MimePart();
    ~MimePart();

    /* [1] --- */


    /* [2] Getters and Setters */

    const QString& getHeader() const;
    const QByteArray& getContent() const;

    void setContent(const QByteArray & content);
    void setHeader(const QString & header);

    void addHeaderLine(const QString & line);

    void setContentId(const QString & cId);
    const QString & getContentId() const;

    void setContentName(const QString & cName);
    const QString & getContentName() const;

    void setContentType(const QString & cType);
    const QString & getContentType() const;

    void setCharset(const QString & charset);
    const QString & getCharset() const;

    void setEncoding(Encoding enc);
    Encoding getEncoding() const;

    /* [2] --- */


    /* [3] Public methods */

    virtual QString toString();

    virtual void prepare();

    /* [3] --- */



protected:

    /* [4] Protected members */

    QString header;
    QByteArray content;

    QString cId;
    QString cName;
    QString cType;
    QString cCharset;
    Encoding cEncoding;

    QString mimeString;
    bool prepared;

    /* [4] --- */
};

#endif // MIMEPART_H
