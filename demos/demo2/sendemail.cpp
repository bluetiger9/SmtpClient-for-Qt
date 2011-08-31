#include "sendemail.h"
#include "ui_sendemail.h"

#include <QFileDialog>
#include <QErrorMessage>

#include <iostream>

using namespace std;

SendEmail::SendEmail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendEmail)
{
    ui->setupUi(this);
}

SendEmail::~SendEmail()
{
    delete ui;
}

EmailAddress* SendEmail::stringToEmail(const QString &str)
{
    int p1 = str.indexOf("<");
    int p2 = str.indexOf(">");

    if (p1 == -1)
    {
        // no name, only email address
        return new EmailAddress(str);
    }
    else
    {
        return new EmailAddress(str.mid(p1 + 1, p2 - p1 - 1), str.left(p1));
    }

}

void SendEmail::on_addAttachment_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);


    if (dialog.exec())
        ui->attachments->addItems(dialog.selectedFiles());


}

void SendEmail::on_sendEmail_clicked()
{
    QString host = ui->host->text();
    int port = ui->port->value();
    bool ssl = ui->ssl->isChecked();
    bool auth = ui->auth->isChecked();
    QString user = ui->username->text();
    QString password = ui->password->text();

    EmailAddress *sender = stringToEmail(ui->sender->text());

    QStringList rcptStringList = ui->recipients->text().split(';');

    QString subject = ui->subject->text();
    QString html = ui->texteditor->toHtml();

    SmtpClient smtp (host, port, ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

    MimeMessage message;

    message.setSender(sender);
    message.setSubject(subject);

    for (int i = 0; i < rcptStringList.size(); ++i)
         message.addRecipient(stringToEmail(rcptStringList.at(i)));

    MimeHtml content;
    content.setHtml(html);

    message.addPart(&content);

    for (int i = 0; i < ui->attachments->count(); ++i)
    {
        message.addPart(new MimeFile(new QFile(ui->attachments->item(i)->text())));
    }

    if (!smtp.connectToHost())
    {
        errorMessage("Connection Failed");
        return;
    }

    if (auth)
        if (!smtp.login(user, password))
        {
            errorMessage("Authentification Failed");
            return;
        }

    if (!smtp.sendMail(message))
    {
        errorMessage("Mail sending failed");
        return;
    }

    smtp.quit();

}

void SendEmail::errorMessage(const QString &message)
{
    QErrorMessage err (this);

    err.showMessage(message);

    err.exec();
}
