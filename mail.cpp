#include "mail.h"
#include <QFileInfo>

Mail::Mail()
{
}

void Mail::setSender(QString address, QString name)
{
    this->sender = new EmailAddress(address, name);
}

void Mail::addRecipient(QString rcpt, QString name)
{
    this->recipients << EmailAddress (rcpt, name);
}

void Mail::setSubject(QString subject)
{
    this->subject = subject;
}

void Mail::setContent(QString text)
{
    this->text = text;
}

EmailAddress & Mail::getSender()
{
    return *sender;
}

QList<EmailAddress> & Mail::getRecipients()
{
    return recipients;
}

QString & Mail::getSubject()
{
    return subject;
}

QString & Mail::getText()
{
    return text;

}

void Mail::addAttachment(QFile *file, QString type)
{
    attachments << Attachment (*file, type);
}

QList<Attachment> & Mail::getAttachments()
{
    return attachments;
}



/****** Email Address *******/
EmailAddress::EmailAddress(QString address, QString name)
{
    this->address = address;
    this->name = name;
}

QString EmailAddress::getAddress() const
{
    return address;
}

QString EmailAddress::getName() const
{
    return name;
}


/****** Attachment *******/
Attachment::Attachment(QFile& file, QString type) :
    file(file)
{
    //this->file = file;
    this->type = type;
    this->name = QFileInfo(file).fileName();
}

QFile& Attachment::getFile() const
{
    return file;
}

QString Attachment::getName() const
{
    return name;
}

QString Attachment::getType() const
{
    return type;
}
