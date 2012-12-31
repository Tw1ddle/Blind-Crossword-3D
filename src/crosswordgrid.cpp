#include "crosswordgrid.h"

GridData::GridData()
{
}

uivec3 GridData::getDimensions() const
{
    return m_GridDimensions;
}

void GridData::setDimensions(uivec3 dimensions)
{
    m_GridDimensions = dimensions;
}

void GridData::push_back(Letter letter)
{
    m_Letters.push_back(letter);
}

Letter* GridData::getRefLetterAt(unsigned int index)
{
    return &m_Letters.at(index);
}

const Letter* GridData::getLetterAt(unsigned int index) const
{
    return &m_Letters.at(index);
}

void GridData::clear()
{
    m_Letters.clear();
    m_GridDimensions = uivec3(0u, 0u, 0u);
}

unsigned int GridData::getSize() const
{
    return m_Letters.size();
}
