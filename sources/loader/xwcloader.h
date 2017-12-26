/*! \brief A loader for .xwc format files.
 */

#pragma once

#include <QStringList>

#include "crossword/crosswordbase.h"
#include "loader/crosswordloader.h"

namespace loader {

class XWCLoader : public CrosswordLoaderInterface {
  public:
    bool loadMetadata(CrosswordBase& puzzle, QStringList& linelist);
    bool loadGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool loadClues(CrosswordBase& puzzle, QStringList& linelist);

    bool saveMetadata(CrosswordBase& puzzle, QStringList& linelist);
    bool saveGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool saveClues(CrosswordBase& puzzle, QStringList& linelist);

  private:
    bool loadAcrossClues(CrosswordBase& puzzle, QStringList& linelist);
    bool loadAwayClues(CrosswordBase& puzzle, QStringList& linelist);
};

}
