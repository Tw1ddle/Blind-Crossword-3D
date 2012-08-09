#include "lettergrid.h"

LetterGrid::LetterGrid()
{
}

Letter LetterGrid::getLetterAt(uivec3 location) const
{
    return m_Letters.at(getIndex(location));
}

void LetterGrid::setLetterAt(QChar letter, uivec3 location)
{
    m_Letters.at(getIndex(location)).setChar(letter);
}

Letter LetterGrid::getLetterAt(unsigned int index) const
{
    return m_Letters.at(index);
}

void LetterGrid::setLetterAt(QChar letter, unsigned int index)
{
    m_Letters.at(index).setChar(letter);
}

uivec3 LetterGrid::getDimensions() const
{
    return m_GridDimensions;
}

void LetterGrid::setDimensions(uivec3 dimensions)
{
    m_GridDimensions = dimensions;
}

void LetterGrid::push_back(Letter letter)
{
    m_Letters.push_back(letter);
}

Letter* LetterGrid::getRefLetterAt(uivec3 location)
{
    return &m_Letters.at(getIndex2(location));
}

void LetterGrid::clear()
{
    m_Letters.clear();
    m_GridDimensions = uivec3();
}

unsigned int LetterGrid::getSize() const
{
    return m_Letters.size();
}

unsigned int LetterGrid::getIndex(uivec3 location) const
{
    return location.getX() + m_GridDimensions.getX() * location.getY() + m_GridDimensions.getX() * m_GridDimensions.getY() * location.getZ();
}

unsigned int LetterGrid::getIndex2(uivec3 location) const
{
    return location.getY() + m_GridDimensions.getY() * location.getX() + m_GridDimensions.getY() * m_GridDimensions.getX() * location.getZ();
}
