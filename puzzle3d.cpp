#include "puzzle3d.h"

Puzzle3D::Puzzle3D()
{
}

void Puzzle3D::setDimensions(uivec3 dimensions)
{
    m_WorkingGrid.setDimensions(dimensions);
}

LetterGrid &Puzzle3D::getRefWorkingGrid()
{
    return m_WorkingGrid;
}

std::vector<CrosswordEntry3D> &Puzzle3D::getRefCrosswordEntries()
{
    return m_CrosswordEntries;
}

void Puzzle3D::clearPuzzle()
{
    m_PuzzleTitle.clear();
    m_AuthorTitle.clear();
    m_PuzzleType.clear();
    m_WorkingGrid.clear();
    m_CrosswordEntries.clear();
}
