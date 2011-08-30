#ifndef MIMEFILE_H
#define MIMEFILE_H

#include "mimepart.h"
#include <QFile>

class MimeFile : public MimePart
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    MimeFile(QFile *f);
    ~MimeFile();

    /* [1] --- */


    /* [2] Getters and Setters */

    void setName(const QString & name);
    void setType(const QString & type);

    const QString & getName() const;
    const QString & getType() const;

    /* [2] --- */

protected:

    /* [3] Protected members */

    QFile* file;
    QString name;
    QString type;

    /* [3] --- */


    /* [4] Protected methods */

    void prepare();

    /* [4] --- */

};

#endif // MIMEFILE_H
