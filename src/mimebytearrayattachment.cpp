#include "mimebytearrayattachment.h"

MimeByteArrayAttachment::MimeByteArrayAttachment(const QString& name, const QByteArray &content) :
    MimePart()
{
    this->cType = "application/octet-stream";
    this->cEncoding = Base64;
    this->cName = name,
    this->content = content;
}

MimeByteArrayAttachment::~MimeByteArrayAttachment() {}

