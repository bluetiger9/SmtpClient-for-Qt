#include "mimefile.h"

#include <QFileInfo>

/* [1] Constructors and Destructors */

MimeFile::MimeFile(QFile *file)
{
    this->file = file;
    this->type = "application/octet-stream";
    this->name = QFileInfo(*file).fileName();
}

MimeFile::~MimeFile()
{
}

/* [1] --- */


/* [2] Getters and setters */

void MimeFile::setName(const QString & name)
{
    this->name = name;
}

void MimeFile::setType(const QString & type)
{
    this->type = type;
}

const QString & MimeFile::getName() const
{
    return name;
}

const QString & MimeFile::getType() const
{
    return type;
}

/* [2] --- */


/* [3] Protected methods */

void MimeFile::prepare()
{
    this->header = "Content-Type: " + type + "; name=" + name + "\n";
    this->header += "Content-Transfer-Encoding: base64\n";

    file->open(QIODevice::ReadOnly);
    this->content = file->readAll().toBase64() + "\n";
    file->close();
}

/* [3] --- */

