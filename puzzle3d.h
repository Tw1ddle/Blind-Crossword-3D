#ifndef PUZZLE3D_H
#define PUZZLED_H

#include <vector>
#include <QString>

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry3d.h"
#include "puzzleloader.h"
#include "direction.h"

class Puzzle3D
{
    friend class PuzzleLoader;

public:
    Puzzle3D();

    uivec3& getDimensions();

    std::vector<Letter>& getRefWorkingLetters();
    std::vector<CrosswordEntry3D>& getRefCrosswordEntries();

    Letter& getRefSolutionLetterAtLocation(uivec3 location);
    Letter& getRefWorkingLetterAtLocation(uivec3 location);

    void clearPuzzle();

private:
    unsigned int getLetterIndex(uivec3 location);

    uivec3 m_GridDimensions;
    std::vector<Letter> m_SolutionLetters;
    std::vector<CrosswordEntry3D> m_CrosswordEntries;
    QString m_PuzzleTitle;
    QString m_AuthorTitle;
    QString m_PuzzleType;

    std::vector<Letter> m_WorkingLetters;
};

#endif // PUZZLE3D_H
