#include "mimebase64formatter.h"

MimeBase64Formatter::MimeBase64Formatter(QIODevice *out) :
    MimeContentFormatter(out) {}

qint64 MimeBase64Formatter::writeData(const char *data, qint64 maxLength) {
    int lines = (maxLength - 1) / lineLength + 1;
    for (int i = 1; i < lines; ++i) {
        output->write(data, lineLength);
        output->write("\r\n");
        data += lineLength;
    }
    output->write(data, maxLength - (lines - 1) * lineLength);
    output->write("\r\n");
    return maxLength;
}
