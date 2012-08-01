#ifndef PUZZLE3D_H
#define PUZZLED_H

#include <vector>
#include <QString>

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry3d.h"
#include "puzzleloader.h"

class Puzzle3D
{
    friend class PuzzleLoader;

public:
    Puzzle3D();

    uivec3& getDimensions();
    std::vector<Letter>& getWorkingLetters();

    void clearPuzzle();

private:
    uivec3 m_GridDimensions;
    std::vector<Letter> m_Letters;
    std::vector<Letter> m_WorkingLetters;
    std::vector<CrosswordEntry3D> m_CrosswordEntries;
    QString m_PuzzleTitle;
    QString m_AuthorTitle;
    QString m_PuzzleType;
};

#endif // PUZZLE3D_H
