#include "mimehtml.h"

/* [1] Constructors and Destructors */

MimeHtml::MimeHtml()
{
    this->html = "";
    this->encoding = PlainText;
    this->charset = "utf-8";
}

MimeHtml::~MimeHtml() {}

/* [1] --- */


/* [2] Getters and Setters */

void MimeHtml::setHtml(const QString & html)
{
    this->html = html;
}

void MimeHtml::setEncoding(MimePart::Encoding enc)
{
    this->encoding = enc;
}

void MimeHtml::setCharset(const QString & charset)
{
    this->charset = charset;
}

const QString & MimeHtml::getHtml() const
{
    return html;
}

const QString & MimeHtml::getCharset() const
{
    return charset;
}

MimePart::Encoding MimeHtml::getEncoding() const
{
    return encoding;
}

/* [2] --- */


/* [3] Protected methods */

void MimeHtml::prepare()
{
    this->header = "Content-Type: text/html; charset="
            + this->charset + "\n";

    this->header += "Content-Transfer-Encoding: ";
    this->header += (this->encoding == PlainText) ? "7bit\n" : "base64\n";

    this->content = (this->encoding == PlainText) ? this->html.toAscii() : QByteArray().append(this->html).toBase64();
    this->content += "\n";
}

/* [3] --- */
