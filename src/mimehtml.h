#ifndef MIMEHTML_H
#define MIMEHTML_H

#include "mimepart.h"

class MimeHtml : public MimePart
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    MimeHtml();
    ~MimeHtml();

    /* [1] --- */


    /* [2] Getters and Setters */

    void setHtml(const QString & html);
    void setEncoding(Encoding enc);
    void setCharset(const QString & charset);

    const QString& getHtml() const;
    const QString& getCharset() const;
    Encoding getEncoding() const;

    /* [2] --- */

protected:

    /* [3] Protected members */

    QString html;
    QString charset;
    Encoding encoding;

    /* [3] --- */


    /* [4] Protected methods */

    void prepare();

    /* [4] --- */
};

#endif // MIMEHTML_H
