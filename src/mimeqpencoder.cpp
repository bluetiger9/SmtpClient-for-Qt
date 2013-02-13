#include "mimeqpencoder.h"
#include "quotedprintable.h"

MimeQpEncoder::MimeQpEncoder() {}

QByteArray MimeQpEncoder::encode(const QByteArray &data) {
    return QuotedPrintable::encode(data).toLatin1();
}
