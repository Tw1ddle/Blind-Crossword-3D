#ifndef XWCLOADER_H
#define XWCLOADER_H

#include <QStringList>

#include "puzzlebase.h"
#include "puzzleloader.h"

class XWCLoader : public PuzzleLoaderInterface
{
public:
    bool loadMetaData(PuzzleBase& puzzle, QStringList& linelist);
    bool loadGrid(PuzzleBase& puzzle, QStringList& linelist);
    bool loadClues(PuzzleBase& puzzle, QStringList& linelist);

    bool saveMetaData(PuzzleBase& puzzle, QStringList& linelist);
    bool saveGrid(PuzzleBase& puzzle, QStringList& linelist);
    bool saveClues(PuzzleBase& puzzle, QStringList& linelist);

private:
    bool loadAcrossClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadAwayClues(PuzzleBase& puzzle, QStringList& linelist);
};

#endif // XWCLOADER_H
