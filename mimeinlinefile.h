#ifndef MIMEINLINEFILE_H
#define MIMEINLINEFILE_H

#include "mimefile.h"

class MimeInlineFile : public MimeFile
{
public:

    /* [1] Constructors and Destructors */

    MimeInlineFile(QFile *f);
    ~MimeInlineFile();

    /* [1] --- */


    /* [2] Getters and Setters */

    void setContentId(const QString & cid);
    const QString & getContentId() const;

    /* [2] --- */

protected:

    /* [3] Protected members */

    QString cid;

    /* [3] --- */


    /* [4] Protected methods */

    void prepare();

    /* [4] --- */
};

#endif // MIMEINLINEFILE_H
