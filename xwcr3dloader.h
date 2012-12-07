/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

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

    bool saveMetaData(PuzzleBase& puzzle, QStringList& linelist);
    bool saveGrid(PuzzleBase& puzzle, QStringList& linelist);
    bool saveClues(PuzzleBase& puzzle, QStringList& linelist);

private:
    bool loadClockwiseClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadAntiClockwiseClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadDiametricClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadDownClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadUpClues(PuzzleBase& puzzle, QStringList& linelist);
    bool loadSnakingClues(PuzzleBase& puzzle, QStringList& linelist);
};

#endif // XWCR3DLOADER_H
