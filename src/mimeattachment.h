#ifndef MIMEATTACHMENT_H
#define MIMEATTACHMENT_H

#include <QFile>
#include "mimepart.h"
#include "mimefile.h"

class MimeAttachment : public MimeFile
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    MimeAttachment(QFile* file);
    ~MimeAttachment();

    /* [1] --- */

protected:

    /* [2] Protected methods */

    void prepare();

    /* [2] --- */
};

#endif // MIMEATTACHMENT_H
