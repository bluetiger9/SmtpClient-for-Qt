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

#ifndef EMAILADDRESS_H
#define EMAILADDRESS_H

#include <QObject>

class EmailAddress : public QObject
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    EmailAddress();
    EmailAddress(const QString & address, const QString & name="");

    ~EmailAddress();

    /* [1] --- */


    /* [2] Getters and Setters */
    void setName(const QString & name);
    void setAddress(const QString & address);

    const QString & getName() const;
    const QString & getAddress() const;

    /* [2] --- */


private:

    /* [3] Private members */

    QString name;
    QString address;

    /* [3] --- */
};

#endif // EMAILADDRESS_H
