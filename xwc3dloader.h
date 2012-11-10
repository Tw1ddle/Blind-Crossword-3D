#ifndef XWC3DLOADER_H
#define XWC3DLOADER_H

#include <QStringList>

#include "puzzlebase.h"
#include "puzzleloader.h"

class XWC3DLoader : public PuzzleLoaderInterface
{
public:
    bool loadMetaData(PuzzleBase& puzzle, QStringList& linelist);
    bool loadGrid(PuzzleBase& puzzle, QStringList& linelist);
    bool loadClues(PuzzleBase& puzzle, QStringList& linelist);

private:
    bool loadAcrossClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadAwayClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadDownClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadUpClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadSnakingClues(PuzzleBase& puzzle, QStringList& linelist);
};

#endif // XWC3DLOADER_H
