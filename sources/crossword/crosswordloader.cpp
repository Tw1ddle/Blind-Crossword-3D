#include "crosswordloader.h"
#include "crosswordbase.h"

#include <cassert>

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>

#include "loaders/xwcloader.h"
#include "loaders/xwc3dloader.h"
#include "util/utilities.h"

CrosswordLoader::CrosswordLoader()
{
}

bool CrosswordLoader::loadPuzzle(CrosswordBase& puzzle, QString filePath, QString extension)
{
    QStringList linelist;

    if (!util::readFile(linelist, filePath)) {
        return false;
    }

    puzzle.clear();

    if (extension == fileformat::XWC3D || extension == fileformat::XWCR3D) {
        XWC3DLoader loader;
        puzzle.m_loaded = readInFile(loader, puzzle, linelist);
    }

    else if (extension == fileformat::XWC) {
        XWCLoader loader;
        puzzle.m_loaded = readInFile(loader, puzzle, linelist);
    }

    if (!puzzle.m_loaded) {
        puzzle.clear();
        return false;
    }

    return true;
}

bool CrosswordLoader::savePuzzle(CrosswordBase& puzzle, QString filePath, QString extension)
{
    if (puzzle.getRefEntries().size() <= 0 || puzzle.getRefGrid().getSize() <= 0 || !puzzle.m_loaded) {
        return false;
    }

    if (puzzle.getRefGrid().getDimensions().getZ() > 1 && (extension != fileformat::XWC3D &&
                                                           extension != fileformat::XWCR3D)) {
        return false;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    if (extension == fileformat::XWC3D || extension == fileformat::XWCR3D) {
        XWC3DLoader loader;
        return writeOutFile(loader, puzzle, file);
    } else if (extension == fileformat::XWC) {
        XWCLoader loader;
        return writeOutFile(loader, puzzle, file);
    }

    return false;
}

bool CrosswordLoader::readInFile(CrosswordLoaderInterface& loader, CrosswordBase& puzzle,
                                 QStringList& linelist)
{
    puzzle.clear();

    if (!loader.loadMetadata(puzzle, linelist)) {
        emit loaderError(tr("Loader error"), tr("Error loading crossword metadata"));
        return false;
    }

    if (!loader.loadGrid(puzzle, linelist)) {
        emit loaderError(tr("Loader error"), tr("Error loading crossword grid"));
        return false;
    }

    if (!loader.loadClues(puzzle, linelist)) {
        emit loaderError(tr("Loader error"), tr("Error loading crossword clues"));
        return false;
    }

    return true;
}

bool CrosswordLoader::writeOutFile(CrosswordLoaderInterface& loader, CrosswordBase& puzzle,
                                   QFile& file)
{
    QStringList linelist;

    if (!loader.saveMetadata(puzzle, linelist)) {
        emit loaderError(tr("Loader error"), tr("Error saving crossword metadata") );
        return false;
    }

    if (!loader.saveGrid(puzzle, linelist)) {
        emit loaderError(tr("Loader error"), tr("Error saving crossword grid"));
        return false;
    }

    if (!loader.saveClues(puzzle, linelist)) {
        emit loaderError(tr("Loader error"), tr("Error saving crossword clues"));
        return false;
    }

    return util::writeToFile(linelist, file);
}


