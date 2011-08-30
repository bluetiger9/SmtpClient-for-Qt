#include "mimeattachment.h"
#include <QFileInfo>

/* [1] Constructors and Destructors */

MimeAttachment::MimeAttachment(QFile *file)
    : MimeFile(file)
{
}

MimeAttachment::~MimeAttachment()
{
}

/* [1] --- */


/* [2] Protected methods */

void MimeAttachment::prepare()
{
    MimeFile::prepare();

    this->header += "Content-disposition: attachment;\n";
}

/* [2] --- */
