#include "mimepart.h"

/* [1] Constructors and Destructors */

MimePart::MimePart()
{
}

MimePart::~MimePart()
{
    return;
}

/* [1] --- */


/* [2] Getters and Setters */

void MimePart::setContent(const QString & content)
{
    this->content = content;
}

void MimePart::setHeader(const QString & header)
{
    this->header = header;
}

void MimePart::addHeaderLine(const QString & line)
{
    this->header += line;
}

const QString& MimePart::getHeader() const
{
    return header;
}

const QString& MimePart::getContent() const
{
    return content;
}

/* [2] --- */


/* [3] Public methods */

QString MimePart::toString()
{
    prepare();
    return header + "\n" + content;
}

/* [3] --- */


/* [4] Protected methods */

void MimePart::prepare() {}

/* [4] --- */
