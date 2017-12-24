/*! \brief A class for loading .xwc3d format crossword puzzle files.
 */

#pragma once

#include <QStringList>

#include "crossword/crosswordbase.h"
#include "crossword/crosswordloader.h"

class XWC3DLoader : public CrosswordLoaderInterface
{
public:
    bool loadMetaData(CrosswordBase& puzzle, QStringList& linelist);
    bool loadGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool loadClues(CrosswordBase& puzzle, QStringList& linelist);

    bool saveMetaData(CrosswordBase& puzzle, QStringList& linelist);
    bool saveGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool saveClues(CrosswordBase& puzzle, QStringList& linelist);

private:
    bool loadXWC3DGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool loadXWCR3DGrid(CrosswordBase& puzzle, QStringList& linelist);

    bool saveXWC3DGrid(CrosswordBase& puzzle, QStringList& linelist);

    bool loadCluesHelper(CrosswordBase& puzzle, QStringList& linelist, QString direction);
    bool loadSnakingClues(CrosswordBase& puzzle, QStringList& linelist, unsigned int numsnaking);

    bool saveCluesHelper(CrosswordBase& puzzle, QStringList& linelist, QString direction);
};
