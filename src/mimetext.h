#ifndef MIMETEXT_H
#define MIMETEXT_H

#include "mimepart.h"

class MimeText : public MimePart
{
public:

    /* [1] Constructors and Destructors */

    MimeText();
    ~MimeText();

    /* [1] --- */


    /* [2] Getters and Setters*/

    void setText(const QString & text);
    void setEncoding(Encoding enc);
    void setCharset(const QString & charset);

    const QString & getText() const;
    MimePart::Encoding getEncoding() const;
    const QString & getCharset() const;

    /* [2] --- */

protected:

    /* [3] Protected members */

    QString text;
    QString charset;
    Encoding encoding;

    /* [3] --- */


    /* [4] Protected methods */

    void prepare();

    /* [4] --- */

};

#endif // MIMETEXT_H
