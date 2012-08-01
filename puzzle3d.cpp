#include "puzzle3d.h"

Puzzle3D::Puzzle3D()
{
    m_GridDimensions = uivec3(3u, 3u, 3u);
}

uivec3& Puzzle3D::getDimensions()
{
    return m_GridDimensions;
}

std::vector<Letter> &Puzzle3D::getWorkingLetters()
{
    return m_Letters;
}

void Puzzle3D::clearPuzzle()
{
    m_PuzzleTitle.clear();
    m_AuthorTitle.clear();
    m_PuzzleType.clear();
    m_Letters.clear();
    m_WorkingLetters.clear();
    m_CrosswordEntries.clear();
    m_GridDimensions = uivec3();
}
