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

#ifndef MIMEMESSAGE_H
#define MIMEMESSAGE_H

#include "mimepart.h"
#include "emailaddress.h"
#include <QList>

class MimeMessage : public QObject
{
public:

    /* [1] Constructors and Destructors */

    MimeMessage();
    ~MimeMessage();

    /* [1] --- */


    /* [2] Getters and Setters */

    void setSender(EmailAddress* e);
    void addRecipient(EmailAddress* rcpt);
    void setSubject(const QString & subject);
    void addPart(MimePart* part);

    void setHeaderEncoding(MimePart::Encoding);

    const EmailAddress & getSender() const;
    const QList<EmailAddress*> & getRecipients() const;
    const QString & getSubject() const;
    const QList<MimePart*> & getParts() const;

    /* [2] --- */


    /* [3] Public methods */

    virtual QString toString();

    /* [3] --- */

protected:

    /* [4] Protected members */

    EmailAddress* sender;
    QList<EmailAddress*> recipients;
    QString subject;
    QList<MimePart*> parts;

    MimePart::Encoding hEncoding;

    /* [4] --- */


};

#endif // MIMEMESSAGE_H
