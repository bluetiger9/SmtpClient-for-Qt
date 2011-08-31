#ifndef SENDEMAIL_H
#define SENDEMAIL_H

#include <QWidget>

#include "../../src/SmtpMime"



namespace Ui {
    class SendEmail;
}

class SendEmail : public QWidget
{
    Q_OBJECT

public:
    explicit SendEmail(QWidget *parent = 0);
    ~SendEmail();

    static EmailAddress * stringToEmail(const QString & str);

private slots:
    void on_addAttachment_clicked();

    void on_sendEmail_clicked();

private:
    Ui::SendEmail *ui;

    void errorMessage(const QString & message);
};

#endif // SENDEMAIL_H
