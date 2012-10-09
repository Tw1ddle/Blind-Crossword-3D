#include "lettergrid.h"

LetterGrid::LetterGrid()
{
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

Letter* LetterGrid::getRefLetterAt(unsigned int index)
{
    return &m_Letters.at(index);
}

const Letter* LetterGrid::getLetterAt(unsigned int index) const
{
    return &m_Letters.at(index);
}

void LetterGrid::clear()
{
    m_Letters.clear();
    m_GridDimensions = uivec3(0u, 0u, 0u);
}

unsigned int LetterGrid::getSize() const
{
    return m_Letters.size();
}
