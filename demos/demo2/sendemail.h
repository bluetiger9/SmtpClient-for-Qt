/*
  Copyright (c) 2011 - Tőkés Attila

  This file is part of SmtpClient for Qt.

  SmtpClient for Qt is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  SmtpClient for Qt is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY.

  See the LICENCE file for more details.
*/

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

    static EmailAddress stringToEmail(const QString & str);

private slots:
    void on_addAttachment_clicked();

    void on_sendEmail_clicked();

private:
    Ui::SendEmail *ui;

    void errorMessage(const QString & message);
};

#endif // SENDEMAIL_H
