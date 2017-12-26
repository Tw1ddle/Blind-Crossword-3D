/*! \brief A loader for .xwc format files.
 */

#pragma once

#include <QStringList>

#include "crossword/crosswordbase.h"
#include "loader/crosswordloader.h"

namespace loader {

class XWCLoader : public CrosswordLoaderInterface {
  public:
    bool loadMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool loadGrid(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool loadClues(crossword::CrosswordBase& puzzle, QStringList& linelist);

    bool saveMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool saveGrid(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool saveClues(crossword::CrosswordBase& puzzle, QStringList& linelist);

  private:
    bool loadAcrossClues(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool loadAwayClues(crossword::CrosswordBase& puzzle, QStringList& linelist);
};

}
