#include "letter.h"

#include <assert.h>

Letter::Letter(QChar letter, uivec3 location)
{
    this->m_Letter = letter;
    this->m_Location = location;
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
