/*
  Copyright (c) 2011-2012 - Tőkés Attila

  This file is part of SmtpClient for Qt.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  See the LICENSE file for more details.
*/

#include "mimemultipart.h"
#include <QIODevice>
#include <QTime>
#include <QCryptographicHash>
#include <QRandomGenerator>

const QString MULTI_PART_NAMES[] = {
    "multipart/mixed",         //    Mixed
    "multipart/digest",        //    Digest
    "multipart/alternative",   //    Alternative
    "multipart/related",       //    Related
    "multipart/report",        //    Report
    "multipart/signed",        //    Signed
    "multipart/encrypted"      //    Encrypted
};

MimeMultiPart::MimeMultiPart(MultiPartType type)
{
    this->type = type;
    this->cType = MULTI_PART_NAMES[this->type];
    this->cEncoding = _8Bit;

    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(QByteArray().append(QRandomGenerator::global()->generate64()));
    cBoundary = md5.result().toHex();
}

MimeMultiPart::~MimeMultiPart() {
    foreach (MimePart *part, parts) {
        delete part;
    }
}

void MimeMultiPart::addPart(MimePart *part) {
    parts.append(part);
}

const QList<MimePart*> & MimeMultiPart::getParts() const {
    return parts;
}

void MimeMultiPart::writeContent(QIODevice &device) const {
    QList<MimePart*>::const_iterator it;

    for (it = parts.constBegin(); it != parts.constEnd(); it++) {
        device.write("--" );
        device.write(cBoundary.toLatin1());
        device.write("\r\n");
        (*it)->writeToDevice(device);
    };

    device.write("--");
    device.write(cBoundary.toLatin1());
    device.write("--\r\n");
}


void MimeMultiPart::setMimeType(const MultiPartType type) {
    this->type = type;
    this->cType = MULTI_PART_NAMES[type];
}

MimeMultiPart::MultiPartType MimeMultiPart::getMimeType() const {
    return type;
}
