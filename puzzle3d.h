#ifndef PUZZLE3D_H
#define PUZZLED_H

#include <vector>
#include <QString>

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry3d.h"
#include "puzzleloader.h"
#include "direction.h"
#include "lettergrid.h"

class Puzzle3D
{
    friend class PuzzleLoader;

public:
    Puzzle3D();

    void setDimensions(uivec3 dimensions);

    LetterGrid& getRefWorkingGrid();
    std::vector<CrosswordEntry3D>& getRefCrosswordEntries();

    void clearPuzzle();

private:
    LetterGrid m_WorkingGrid;

    std::vector<CrosswordEntry3D> m_CrosswordEntries;
    QString m_PuzzleTitle;
    QString m_AuthorTitle;
    QString m_PuzzleType;
};

#endif // PUZZLE3D_H
