#include "puzzleloader.h"
#include "puzzlebase.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>
#include <assert.h>

#include "xwcloader.h"
#include "xwc3dloader.h"

#include "utilities.h"

PuzzleLoader::PuzzleLoader()
{
}

bool PuzzleLoader::loadPuzzle(PuzzleBase &puzzle, QString filePath, QString extension)
{
    QStringList linelist;

    if(!Utilities::readFile(linelist, filePath))
    {
        return false;
    }

    puzzle.clear();

    if(extension == FileFormats::XWC3D || extension == FileFormats::XWCR3D)
    {
        puzzle.m_CrosswordLoaded = readInFile(XWC3DLoader(), puzzle, linelist);
    }

    else if(extension == FileFormats::XWC)
    {
        puzzle.m_CrosswordLoaded = readInFile(XWCLoader(), puzzle, linelist);
    }
    if(!puzzle.m_CrosswordLoaded)
    {
        puzzle.clear();
        return false;
    }

    return true;
}

bool PuzzleLoader::savePuzzle(PuzzleBase &puzzle, QString filePath, QString extension)
{
    if(puzzle.getRefCrosswordEntries().size() <= 0 || puzzle.getRefGrid().getSize() <= 0 || !puzzle.m_CrosswordLoaded)
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
        return writeOutFile(XWC3DLoader(), puzzle, file);
    }
    else if(extension == FileFormats::XWC)
    {
        return writeOutFile(XWCLoader(), puzzle, file);
    }

    return false;
}

bool PuzzleLoader::readInFile(PuzzleLoaderInterface& loader, PuzzleBase& puzzle, QStringList& linelist)
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

bool PuzzleLoader::writeOutFile(PuzzleLoaderInterface &loader, PuzzleBase &puzzle, QFile& file)
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


