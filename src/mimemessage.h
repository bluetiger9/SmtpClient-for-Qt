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

    const EmailAddress & getSender() const;
    const QList<EmailAddress*> & getRecipients() const;
    const QString & getSubject() const;
    const QList<MimePart*> & getParts() const;

    /* [2] --- */


    /* [3] Public methods */

    QString toString();

    /* [3] --- */

protected:

    /* [4] Protected members */

    EmailAddress* sender;
    QList<EmailAddress*> recipients;
    QString subject;
    QList<MimePart*> parts;

    /* [4] --- */


};

#endif // MIMEMESSAGE_H
