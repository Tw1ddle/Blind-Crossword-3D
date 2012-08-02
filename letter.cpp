#include "letter.h"

#include <assert.h>

Letter::Letter(QChar letter, uivec3 location)
{
    this->m_Letter = letter;
    this->m_Location = location;
}

QChar Letter::getLetter()
{
    return m_Letter.toUpper();
}

uivec3 Letter::getLocation()
{
    return m_Location;
}
