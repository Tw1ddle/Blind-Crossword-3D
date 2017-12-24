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

bool CrosswordLoader::loadPuzzle(CrosswordBase &puzzle, QString filePath, QString extension)
{
    QStringList linelist;

    if(!Utilities::readFile(linelist, filePath))
    {
        return false;
    }

    puzzle.clear();

    if(extension == FileFormats::XWC3D || extension == FileFormats::XWCR3D)
    {
        XWC3DLoader loader;
        puzzle.m_Loaded = readInFile(loader, puzzle, linelist);
    }

    else if(extension == FileFormats::XWC)
    {
        XWCLoader loader;
        puzzle.m_Loaded = readInFile(loader, puzzle, linelist);
    }
    if(!puzzle.m_Loaded)
    {
        puzzle.clear();
        return false;
    }

    return true;
}

bool CrosswordLoader::savePuzzle(CrosswordBase &puzzle, QString filePath, QString extension)
{
    if(puzzle.getRefEntries().size() <= 0 || puzzle.getRefGrid().getSize() <= 0 || !puzzle.m_Loaded)
    {
        return false;
    }

    if(puzzle.getRefGrid().getDimensions().getZ() > 1 && (extension != FileFormats::XWC3D && extension != FileFormats::XWCR3D))
    {
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    if(extension == FileFormats::XWC3D || extension == FileFormats::XWCR3D)
    {
        XWC3DLoader loader;
        return writeOutFile(loader, puzzle, file);
    }
    else if(extension == FileFormats::XWC)
    {
        XWCLoader loader;
        return writeOutFile(loader, puzzle, file);
    }

    return false;
}

bool CrosswordLoader::readInFile(CrosswordLoaderInterface& loader, CrosswordBase& puzzle, QStringList& linelist)
{
    puzzle.clear();

    if(!loader.loadMetaData(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword metadata")));
        return false;
    }
    if(!loader.loadGrid(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword grid")));
        return false;
    }
    if(!loader.loadClues(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword clues")));
        return false;
    }

    return true;
}

bool CrosswordLoader::writeOutFile(CrosswordLoaderInterface &loader, CrosswordBase &puzzle, QFile& file)
{
    QStringList linelist;

    if(!loader.saveMetaData(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error saving crossword metadata")));
        return false;
    }
    if(!loader.saveGrid(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error saving crossword grid")));
        return false;
    }
    if(!loader.saveClues(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error saving crossword clues")));
        return false;
    }

    return Utilities::writeToFile(linelist, file);
}


