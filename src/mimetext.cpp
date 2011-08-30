#include "mimetext.h"

/* [1] Constructors and Destructors */

MimeText::MimeText()
{
    this->charset = "utf-8";
    this->encoding = PlainText;
}

MimeText::~MimeText() { }

/* [1] --- */


/* [2] Getters and Setters */

void MimeText::setText(const QString & text)
{
    this->text = text;
}

void MimeText::setEncoding(MimePart::Encoding enc)
{
    this->encoding = enc;
}

void MimeText::setCharset(const QString & charset)
{
    this->charset = charset;
}

const QString & MimeText::getText() const
{
    return text;
}

MimePart::Encoding MimeText::getEncoding() const
{
    return encoding;
}

const QString & MimeText::getCharset() const
{
    return charset;
}

/* [2] --- */


/* [3] Protected Methods */

void MimeText::prepare()
{
    this->header = "Content-Type: text/plain; charset: "
            + this->charset + "\n";

    this->header += "Content-Transfer-Encoding: ";
    this->header += (this->encoding == PlainText) ? "7bit\n" : "base64\n";

    this->content = (this->encoding == PlainText) ? this->text.toAscii() : QByteArray().append(this->text).toBase64();
    this->content += "\n";

}

/* [3] --- */
