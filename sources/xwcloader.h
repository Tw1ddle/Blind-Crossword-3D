/*! \brief A loader for .xwc format files.
 */

#pragma once

#include <QStringList>

#include "crosswordbase.h"
#include "crosswordloader.h"

class XWCLoader : public CrosswordLoaderInterface
{
public:
    bool loadMetaData(CrosswordBase& puzzle, QStringList& linelist);
    bool loadGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool loadClues(CrosswordBase& puzzle, QStringList& linelist);

    bool saveMetaData(CrosswordBase& puzzle, QStringList& linelist);
    bool saveGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool saveClues(CrosswordBase& puzzle, QStringList& linelist);

private:
    bool loadAcrossClues(CrosswordBase& puzzle, QStringList& linelist);
    bool loadAwayClues(CrosswordBase& puzzle, QStringList& linelist);
};
