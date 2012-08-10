#include "puzzleloader.h"
#include "puzzle3d.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>
#include <assert.h>

#include "direction.h"

PuzzleLoader::PuzzleLoader()
{
}

void PuzzleLoader::loadPuzzle(BCrossword3D &puzzle, QString filePath, QString extension)
{
    puzzle.clear();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    QString currentLine;
    QStringList linelist;

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
    }
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
        return writeToFile(saveAsXWC(puzzle));
    }
    else if(extension == FileFormats::XWC)
    {
        return writeToFile(saveAsXWC3D(puzzle));
    }

    return false;
}

bool PuzzleLoader::readInXWC3D(BCrossword3D &puzzle, QStringList& linelist)
{
    static const QString errorTitle = tr("XWC3D crossword load error");

    puzzle.m_PuzzleTitle = linelist.takeFirst();
    puzzle.m_AuthorTitle = linelist.takeFirst();
    puzzle.m_PuzzleType = linelist.takeFirst();

    if(puzzle.m_PuzzleTitle.isNull() || puzzle.m_AuthorTitle.isNull() || puzzle.m_PuzzleType.isNull())
    {
        emit(loaderError(errorTitle, tr("Missing author name, author title or puzzle type specifier")));
        return false;
    }

    unsigned int gridY = linelist.takeFirst().toUInt();
    unsigned int gridX = linelist.takeFirst().toUInt();
    unsigned int gridZ = linelist.takeFirst().toUInt();
    puzzle.setDimensions(uivec3(gridX, gridY, gridZ));

    if(gridY <= 0 || gridX <= 0 || gridZ <= 0)
    {
        emit(loaderError(errorTitle, tr("Invalid crossword grid dimensions")));
        return false;
    }

    for(unsigned int z = 0; z < gridZ; z++)
    {
        for(unsigned int x = 0; x < gridX; x++)
        {
            QString currentLine = linelist.takeFirst();

            for(unsigned int ch = 0; ch < gridY; ch++)
            {
                if(currentLine.at(ch) == '1')
                {
                    puzzle.getRefGrid().push_back(Letter(QChar(), uivec3(x % gridZ , ch, z)));
                }
                else
                {
                    puzzle.getRefGrid().push_back(Letter(QChar(46), uivec3(x % gridZ , ch, z)));
                }
            }
        }
    }

    if(puzzle.getRefGrid().getSize() != (gridX * gridY * gridZ))
    {
        emit(loaderError(errorTitle, tr("Invalid crossword grid layout")));
        return false;
    }

    unsigned int numAcross = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numAcross; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::ACROSS);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt() - 1;
        unsigned int posY = list.takeFirst().toUInt() - 1;
        unsigned int posZ = list.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setY(letterPosition.getY() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            emit(loaderError(errorTitle, tr("Invalid clue entry in file")));
            return false;
        }

       Clue clue(list.takeFirst());

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, clue));
    }

    unsigned int numAway = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numAway; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::AWAY);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt() - 1;
        unsigned int posY = list.takeFirst().toUInt() - 1;
        unsigned int posZ = list.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setX(letterPosition.getX() - j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            emit(loaderError(errorTitle, tr("Invalid clue entry in file")));
            return false;
        }

        Clue clue(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, clue));
    }

    unsigned int numThrough = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numThrough; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::DOWN);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt() - 1;
        unsigned int posY = list.takeFirst().toUInt() - 1;
        unsigned int posZ = list.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setZ(letterPosition.getZ() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            emit(loaderError(errorTitle, tr("Invalid clue entry in file")));
            return false;
        }

        Clue clue(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, clue));
    }

    return true;
}

bool PuzzleLoader::readInXWC(BCrossword3D &puzzle, QStringList& linelist)
{
    QString errorTitle = tr("XWC format crossword load error");

    puzzle.m_PuzzleTitle = linelist.takeFirst();
    puzzle.m_AuthorTitle = linelist.takeFirst();
    puzzle.m_PuzzleType = linelist.takeFirst();

    if(puzzle.m_PuzzleTitle.isNull() || puzzle.m_AuthorTitle.isNull() || puzzle.m_PuzzleType.isNull())
    {
        emit(loaderError(errorTitle, tr("Missing author name, author title or puzzle type specifier")));
        return false;
    }

    unsigned int gridY = linelist.takeFirst().toUInt();
    unsigned int gridX = linelist.takeFirst().toUInt();
    puzzle.setDimensions(uivec3(gridX, gridY, 1));

    if(gridY <= 0 || gridX <= 0)
    {
        emit(loaderError(errorTitle, tr("Invalid crossword grid dimensions")));
        return false;
    }

    for(unsigned int x = 0; x < gridX; x++)
    {
        QString currentLine = linelist.takeFirst();

        for(unsigned int ch = 0; ch < gridY; ch++)
        {
            if(currentLine.at(ch) == '1')
            {
                puzzle.getRefGrid().push_back(Letter(QChar(), uivec3(x , ch, 0)));
            }
            else
            {
                puzzle.getRefGrid().push_back(Letter(QChar(46), uivec3(x , ch, 0)));
            }
        }
    }

    if(puzzle.getRefGrid().getSize() != (gridX * gridY))
    {
        emit(loaderError(errorTitle, tr("Invalid crossword grid layout")));
        return false;
    }

    unsigned int numAcross = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numAcross; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::ACROSS);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt() - 1;
        unsigned int posY = list.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, 0);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setY(letterPosition.getY() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            emit(loaderError(errorTitle, tr("Invalid clue entry in file")));
            return false;
        }

       Clue clue(list.takeFirst());

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, clue));
    }

    unsigned int numAway = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numAway; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::AWAY);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt() - 1;
        unsigned int posY = list.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, 0);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setX(letterPosition.getX() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            emit(loaderError(errorTitle, tr("Invalid clue entry in file")));
            return false;
        }

        Clue clue(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, clue));
    }

    return true;
}

QStringList PuzzleLoader::saveAsXWC(BCrossword3D &puzzle)
{
    QStringList linelist;
    linelist.push_back(puzzle.m_PuzzleTitle);
    linelist.push_back(puzzle.m_AuthorTitle);
    linelist.push_back(puzzle.m_PuzzleType);
    linelist.push_back(QString::number(puzzle.getRefGrid().getDimensions().getX()));
    linelist.push_back(QString::number(puzzle.getRefGrid().getDimensions().getY()));



    return linelist;
}

QStringList PuzzleLoader::saveAsXWC3D(BCrossword3D &puzzle)
{
    QStringList linelist;
    linelist.push_back(puzzle.m_PuzzleTitle);
    linelist.push_back(puzzle.m_AuthorTitle);
    linelist.push_back(puzzle.m_PuzzleType);
    linelist.push_back(QString::number(puzzle.getRefGrid().getDimensions().getX()));
    linelist.push_back(QString::number(puzzle.getRefGrid().getDimensions().getY()));
    linelist.push_back(QString::number(puzzle.getRefGrid().getDimensions().getZ()));



    return linelist;
}

bool PuzzleLoader::writeToFile(QStringList& linelist)
{
    return false;
}

QStringList PuzzleLoader::saveXWC3DCrosswordEntryBlock(QStringList &linelist, Direction entryDirection)
{
    return QStringList();
}
