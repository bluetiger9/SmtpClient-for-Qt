#include "mimeqpformatter.h"

MimeQPFormatter::MimeQPFormatter(QIODevice *output) :
    MimeContentFormatter(output) {}

qint64 MimeQPFormatter::writeData(const char *data, qint64 maxLength) {
    int chars = 0;
    const char *start = data;
    for (int i = 0; i < maxLength; ++i) {
        chars++;
        if (data[i] == '\n') {
            output->write(start, chars);
            start += chars;
            chars = 0;
        } else if ((chars > lineLength - 3) && (data[i] == '=')) {
            output->write(start, chars - 1);
            output->write("=\r\n=");
            start += chars;
            chars = 0;
        } else if (chars == lineLength - 1) {
            output->write(start, chars);
            output->write("=\r\n");
            start += chars;
            chars = 0;
        }
    }
    output->write(start, chars);
    return maxLength;
}
