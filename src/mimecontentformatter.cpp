#include "mimecontentformatter.h"

MimeContentFormatter::MimeContentFormatter(QIODevice *out, int length) :
    output(out),
    lineLength(length)
{
    QIODevice::open(WriteOnly);
}

int MimeContentFormatter::getLineLength() const {
    return lineLength;
}

void MimeContentFormatter::setLineLength(int l) {
    lineLength = l;
}

qint64 MimeContentFormatter::readData(char*, qint64) {
    return -1;
}
