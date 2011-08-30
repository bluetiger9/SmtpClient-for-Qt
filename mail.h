#ifndef MAIL_H
#define MAIL_H

#include <QObject>
#include <QFile>

class EmailAddress;
class Attachment;

class Mail : public QObject
{
    Q_OBJECT
public:
    Mail();

    enum ContentType
    {
        Html,
        PlainText
    };


    void setSender(QString sender, QString name="");
    void setSender(EmailAddress adr);

    void addRecipient(QString rcpt, QString name="");
    void setSubject(QString title);
    void setContent(QString text);
    void setEncoding(QString text);
    void addAttachment(QFile* file, QString type="application/octet-stream");

    EmailAddress& getSender();
    QList<EmailAddress>& getRecipients();
    QString& getSubject();
    QString& getText();
    QList<Attachment>& getAttachments();


private:
    EmailAddress* sender;
    QList<EmailAddress> recipients;
    QString subject;
    QString text;
    QList<Attachment> attachments;

};
/*
class EmailAddress
{
public:
    EmailAddress(QString address, QString name="");

    QString getName() const;
    QString getAddress() const;

private:
    QString name;
    QString address;
};
*/

class Attachment
{
public:
    Attachment(QFile& file, QString type = "application/octet-stream");

    QString getName() const;
    QFile& getFile() const;
    QString getType() const;

    void setName(QString name);
    void setType(QString type);

private:
    QFile& file;
    QString name;
    QString type;
};


#endif // MAIL_H
