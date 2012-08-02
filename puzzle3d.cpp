#include "puzzle3d.h"

Puzzle3D::Puzzle3D()
{
    m_GridDimensions = uivec3();
}

uivec3& Puzzle3D::getDimensions()
{
    return m_GridDimensions;
}

std::vector<Letter> &Puzzle3D::getRefWorkingLetters()
{
    return m_SolutionLetters;
}

std::vector<CrosswordEntry3D> &Puzzle3D::getRefCrosswordEntries()
{
    return m_CrosswordEntries;
}

Letter &Puzzle3D::getRefSolutionLetterAtLocation(uivec3 location)
{
    unsigned int index = getLetterIndex(location);
    return m_SolutionLetters.at(index);
}

Letter &Puzzle3D::getRefWorkingLetterAtLocation(uivec3 location)
{
    unsigned int index = getLetterIndex(location);
    return m_WorkingLetters.at(index);
}

void Puzzle3D::clearPuzzle()
{
    m_PuzzleTitle.clear();
    m_AuthorTitle.clear();
    m_PuzzleType.clear();
    m_SolutionLetters.clear();
    m_WorkingLetters.clear();
    m_CrosswordEntries.clear();
    m_GridDimensions = uivec3();
}

unsigned int Puzzle3D::getLetterIndex(uivec3 location)
{
    return location.getX() + (m_GridDimensions.getX() * location.getY()) + (m_GridDimensions.getX() * m_GridDimensions.getY() * location.getZ());
}
