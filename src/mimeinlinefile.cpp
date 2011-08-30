#include "mimeinlinefile.h"

/* [1] Constructors and Destructors */

MimeInlineFile::MimeInlineFile(QFile *f)
    : MimeFile(f)
{
    return;
}

MimeInlineFile::~MimeInlineFile()
{}

/* [1] --- */


/* [2] Getters and Setters */

void MimeInlineFile::setContentId(const QString & cid)
{
    this->cid = cid;
}

const QString & MimeInlineFile::getContentId() const
{
    return cid;
}

/* [2] --- */


/* [3] Protected methods */

void MimeInlineFile::prepare()
{
    MimeFile::prepare();

    this->header += "Content-id: <" + cid + ">\n";
    this->header += "Content-disposition: inline\n";
}

/* [3] --- */



