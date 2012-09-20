#include "letter.h"

Letter::Letter(QChar letter, uivec3 location)
{
    m_Letter = letter;
    m_Location = location;
}

QChar Letter::getChar() const
{
    return m_Letter.toUpper();
}

void Letter::setChar(QChar letter)
{
    m_Letter = letter;
}

uivec3 Letter::getLocation() const
{
    return m_Location;
}

bool Letter::operator ==(const Letter &letter) const
{
    return (this == &letter);
}
