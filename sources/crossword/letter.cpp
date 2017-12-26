#include "crossword/letter.h"

Letter::Letter(QChar letter, util::uivec3 location)
{
    m_letter = letter;
    m_location = location;
}

QChar Letter::getChar() const
{
    return m_letter;
}

void Letter::setChar(QChar letter)
{
    m_letter = letter;
}

util::uivec3 Letter::getLocation() const
{
    return m_location;
}

bool Letter::operator ==(const Letter& letter) const
{
    return this == &letter;
}
