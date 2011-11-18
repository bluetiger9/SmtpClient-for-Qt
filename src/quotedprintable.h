/*
  Copyright (c) 2011 - Tőkés Attila (tokes_atti@yahoo.com)

  This project is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This project is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY.

  See the LICENSE file for more details.
*/

#ifndef QUOTEDPRINTABLE_H
#define QUOTEDPRINTABLE_H

#include <QObject>
#include <QByteArray>

class QuotedPrintable : public QObject
{
    Q_OBJECT
public:

    static QString& encode(const QByteArray &input);
    static QByteArray& decode(const QString &input);

private:
    QuotedPrintable();
};

#endif // QUOTEDPRINTABLE_H
