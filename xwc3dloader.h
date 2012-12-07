/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

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

    bool saveMetaData(PuzzleBase& puzzle, QStringList& linelist);
    bool saveGrid(PuzzleBase& puzzle, QStringList& linelist);
    bool saveClues(PuzzleBase& puzzle, QStringList& linelist);

private:
    bool loadCluesHelper(PuzzleBase& puzzle, QStringList& linelist, QString direction);
    bool loadSnakingClues(PuzzleBase& puzzle, QStringList& linelist, unsigned int numsnaking);
};

#endif // XWC3DLOADER_H
