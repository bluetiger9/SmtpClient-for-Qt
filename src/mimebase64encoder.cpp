#include "mimebase64encoder.h"

MimeBase64Encoder::MimeBase64Encoder() {}

QByteArray MimeBase64Encoder::encode(const QByteArray &data) {
    return data.toBase64();
}
