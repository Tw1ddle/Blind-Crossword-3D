#include "lettergrid.h"

LetterGrid::LetterGrid()
{
}

Letter LetterGrid::getLetterAtLocation(uivec3 location) const
{
    return m_Letters.at(getIndex(location));
}

void LetterGrid::setLetterAtLocation(QChar letter, uivec3 location)
{
    m_Letters.at(getIndex(location)).setChar(letter);
}

Letter LetterGrid::getLetterAtIndex(unsigned int index) const
{
    return m_Letters.at(index);
}

void LetterGrid::setLetterAtIndex(QChar letter, unsigned int index)
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

void LetterGrid::clear()
{
    m_Letters.clear();
    m_GridDimensions = uivec3();
}

unsigned int LetterGrid::getIndex(uivec3 location) const
{
    unsigned int index = location.getX() + (m_GridDimensions.getX() * location.getY()) + (m_GridDimensions.getX() * m_GridDimensions.getY() * location.getZ());
    return index;
}


unsigned int LetterGrid::gridToIndex(uivec3 gridCoordinate, uivec3 gridDimensions)
{
    unsigned int index = gridCoordinate.getX() + (gridDimensions.getX() * gridCoordinate.getY()) + (gridDimensions.getX() * gridDimensions.getY() * gridCoordinate.getZ());

    return index;
}

uivec3 LetterGrid::indexToGrid(unsigned int index, uivec3 gridDimensions)
{
    return uivec3();
}
