#include "puzzleloader.h"
#include "puzzle3d.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>
#include <assert.h>

#include "direction.h"

#include "xwcloader.h"
#include "xwc3dloader.h"

PuzzleLoader::PuzzleLoader()
{
}

bool PuzzleLoader::loadPuzzle(BCrossword3D &puzzle, QString filePath, QString extension)
{
    puzzle.clear();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(&file);
    QString currentLine;
    QStringList linelist;

    if(in.atEnd())
    {
        return false;
    }

    do
    {
        currentLine = in.readLine();
        if(currentLine.length() != 0)
        {
            linelist << currentLine;
        }
    } while (!currentLine.isNull());

    if(extension == FileFormats::XWC3D)
    {
         puzzle.m_CrosswordLoaded = readInXWC3D(puzzle, linelist);
    }
    else if(extension == FileFormats::XWC)
    {
         puzzle.m_CrosswordLoaded = readInXWC(puzzle, linelist);
    }

    if(!puzzle.m_CrosswordLoaded)
    {
        puzzle.clear();
        return false;
    }

    return true;
}

bool PuzzleLoader::savePuzzle(BCrossword3D &puzzle, QString filePath, QString extension)
{
    if(puzzle.getRefCrosswordEntries().size() <= 0 || puzzle.getRefGrid().getSize() <= 0 || !puzzle.m_CrosswordLoaded)
    {
        emit(loaderError(tr("Save error"), tr("There is no puzzle loaded, so there is no puzzle to save")));
        return false;
    }

    if(puzzle.getRefGrid().getDimensions().getZ() > 1 && extension != FileFormats::XWC3D)
    {
        emit(loaderError(tr("Save error"), tr("The current puzzle is a 3D puzzle, and cannot be saved in a 2D puzzle format")));
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit(loaderError(tr("File error"), tr("Failed to open save file for writing")));
        return false;
    }

    if(extension == FileFormats::XWC3D)
    {
        return writeToFile(saveAsXWC3D(puzzle), file);
    }
    else if(extension == FileFormats::XWC)
    {
        return writeToFile(saveAsXWC(puzzle), file);
    }
    else
    {
        emit(loaderError(tr("File format error"), tr("File format not recognised")));
    }

    return false;
}

bool PuzzleLoader::readInXWC(BCrossword3D &puzzle, QStringList& linelist)
{
    XWCLoader xwcLoader;

    if(!xwcLoader.loadMetaData(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword metadata")));
        return false;
    }
    if(!xwcLoader.loadGrid(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword grid")));
        return false;
    }
    if(!xwcLoader.loadClues(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword clues")));
        return false;
    }

    return true;
}

QStringList PuzzleLoader::saveAsXWC(BCrossword3D &puzzle)
{
    XWCLoader xwcLoader;

    QStringList linesToSave;

    if(!xwcLoader.saveMetaData(puzzle, linesToSave))
    {
        emit(loaderError(tr("Loader error"), tr("Error saving crossword clues")));
    }
    if(!xwcLoader.saveGrid(puzzle, linesToSave))
    {
        emit(loaderError(tr("Loader error"), tr("Error saving crossword clues")));
    }
    if(!xwcLoader.saveClues(puzzle, linesToSave))
    {
        emit(loaderError(tr("Loader error"), tr("Error saving crossword clues")));
    }

    return linesToSave;
}

bool PuzzleLoader::readInXWC3D(BCrossword3D &puzzle, QStringList& linelist)
{
    XWC3DLoader xwc3dLoader;

    if(!xwc3dLoader.loadMetaData(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword metadata")));
        return false;
    }
    if(!xwc3dLoader.loadGrid(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword grid")));
        return false;
    }
    if(!xwc3dLoader.loadClues(puzzle, linelist))
    {
        emit(loaderError(tr("Loader error"), tr("Error loading crossword clues")));
        return false;
    }

    return true;
}

QStringList PuzzleLoader::saveAsXWC3D(BCrossword3D &puzzle)
{
    return QStringList();
}

bool PuzzleLoader::writeToFile(QStringList& linelist, QFile& file)
{
    QTextStream out(&file);

    while(!linelist.isEmpty())
    {
        out << linelist.takeFirst();
        out << "\n";
    }

    if(out.status() == QTextStream::Ok)
    {
        return true;
    }

    return false;
}

QStringList PuzzleLoader::saveXWC3DCrosswordEntryBlock(QStringList &linelist, Direction entryDirection)
{
    return QStringList();
}
