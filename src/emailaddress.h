#ifndef EMAILADDRESS_H
#define EMAILADDRESS_H

#include <QObject>

class EmailAddress : public QObject
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    EmailAddress();
    EmailAddress(const QString & address, const QString & name="");

    ~EmailAddress();

    /* [1] --- */


    /* [2] Getters and Setters */
    void setName(const QString & name);
    void setAddress(const QString & address);

    const QString & getName() const;
    const QString & getAddress() const;

    /* [2] --- */


private:

    /* [3] Private members */

    QString name;
    QString address;

    /* [3] --- */
};

#endif // EMAILADDRESS_H
