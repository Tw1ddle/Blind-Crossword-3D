#include "crosswordgrid.h"

GridData::GridData()
{
}

uivec3 GridData::getDimensions() const
{
    return m_gridDimensions;
}

void GridData::setDimensions(uivec3 dimensions)
{
    m_gridDimensions = dimensions;
}

void GridData::push_back(Letter letter)
{
    m_letters.push_back(letter);
}

Letter* GridData::getRefLetterAt(unsigned int index)
{
    return &m_letters.at(index);
}

const Letter* GridData::getLetterAt(unsigned int index) const
{
    return &m_letters.at(index);
}

void GridData::clear()
{
    m_letters.clear();
    m_gridDimensions = uivec3(0u, 0u, 0u);
}

std::size_t GridData::getSize() const
{
    return m_letters.size();
}
