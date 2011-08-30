#ifndef MIMEPART_H
#define MIMEPART_H

#include <QObject>

class MimePart : public QObject
{
    Q_OBJECT
public:

    /* [0] Enumerations */

    enum Encoding {
        PlainText,
        Base64,
        _7Bit,
        _8Bit,
        QuotedPrintable // not implemented
    };

    /* [0] --- */


    /* [1] Constructors and Destructors */

    MimePart();
    ~MimePart();

    /* [1] --- */


    /* [2] Getters and Setters */

    const QString& getHeader() const;
    const QString& getContent() const;

    void setContent(const QString & content);
    void setHeader(const QString & header);

    void addHeaderLine(const QString & line);

    /* [2] --- */


    /* [3] Public methods */

    QString toString();

    /* [3] --- */

protected:

    /* [4] Protected members */

    QString header;
    QString content;

    /* [4] --- */


    /* [5] Protected methods */

    virtual void prepare();

    /* [5] --- */
};

#endif // MIMEPART_H
