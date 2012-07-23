#ifndef MIMECONTENTFORMATTER_H
#define MIMECONTENTFORMATTER_H

#include <QObject>
#include <QByteArray>

class MimeContentFormatter : public QObject
{
    Q_OBJECT
public:
    MimeContentFormatter (int max_length = 76);

    void setMaxLength(int l);

    QString format(const QString &content, bool quotedPrintable = false);

protected:
    int max_length;

};

#endif // MIMECONTENTFORMATTER_H
