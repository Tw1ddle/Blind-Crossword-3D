/*! \brief A class for loading .xwc3d format crossword puzzle files.
 */

#pragma once

#include <QStringList>

#include "crossword/crosswordbase.h"
#include "loader/crosswordloader.h"

namespace loader {

class XWC3DLoader : public CrosswordLoaderInterface {
  public:
    bool loadMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool loadGrid(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool loadClues(crossword::CrosswordBase& puzzle, QStringList& linelist);

    bool saveMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool saveGrid(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool saveClues(crossword::CrosswordBase& puzzle, QStringList& linelist);

  private:
    bool loadXWC3DGrid(crossword::CrosswordBase& puzzle, QStringList& linelist);
    bool loadXWCR3DGrid(crossword::CrosswordBase& puzzle, QStringList& linelist);

    bool saveXWC3DGrid(crossword::CrosswordBase& puzzle, QStringList& linelist);

    bool loadCluesHelper(crossword::CrosswordBase& puzzle, QStringList& linelist, QString direction);
    bool loadSnakingClues(crossword::CrosswordBase& puzzle, QStringList& linelist,
                          unsigned int numsnaking);

    bool saveCluesHelper(crossword::CrosswordBase& puzzle, QStringList& linelist, QString direction);
};

}
