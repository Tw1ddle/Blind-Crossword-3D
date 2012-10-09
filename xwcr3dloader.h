#ifndef XWCR3DLOADER_H
#define XWCR3DLOADER_H

#include <QStringList>

#include "puzzlebase.h"
#include "puzzleloader.h"

class XWCR3DLoader : public PuzzleLoaderInterface
{
public:
    XWCR3DLoader();

public:
    bool loadMetaData(PuzzleBase& puzzle, QStringList& linelist);
    bool loadGrid(PuzzleBase& puzzle, QStringList& linelist);
    bool loadClues(PuzzleBase& puzzle, QStringList& linelist);

private:
    bool loadClockwiseClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadAntiClockwiseClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadDiametricClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadDownClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadUpClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadWindingClues(PuzzleBase& puzzle, QStringList& linelist);
};

#endif // XWCR3DLOADER_H
