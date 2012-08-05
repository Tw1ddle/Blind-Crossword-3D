#include "puzzleloader.h"
#include "puzzle3d.h"

#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QList>
#include <QStringList>
#include <QString>

#include <assert.h>

PuzzleLoader::PuzzleLoader()
{
}

void PuzzleLoader::loadPuzzle(Puzzle3D &puzzle, QString filePath)
{
    puzzle.clearPuzzle();

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

    FileFormats::FORMAT fileFormat = determineFormat(linelist);

    if(fileFormat == FileFormats::XWC3D)
    {
        readInXWC3D(puzzle, linelist);
    }
    else if(fileFormat == FileFormats::XWC)
    {
        readInXWC(puzzle, linelist);
    }
}

void PuzzleLoader::savePuzzle(Puzzle3D &puzzle, QString filePath)
{
}

void PuzzleLoader::readInXWC3D(Puzzle3D &puzzle, QStringList& linelist)
{
    puzzle.m_PuzzleTitle = linelist.takeFirst();
    puzzle.m_AuthorTitle = linelist.takeFirst();
    puzzle.m_PuzzleType = linelist.takeFirst();

    unsigned int gridX = linelist.takeFirst().toUInt();
    unsigned int gridY = linelist.takeFirst().toUInt();
    unsigned int gridZ = linelist.takeFirst().toUInt();
    puzzle.setDimensions(uivec3(gridX, gridY, gridZ));

    for(unsigned int line = 0; line < (gridY * gridZ); line++)
    {
        QString currentLine = linelist.takeFirst();

        for(unsigned int ch = 0; ch < gridX; ch++)
        {
            if(currentLine.at(ch) == '1')
            {
                puzzle.getRefWorkingGrid().push_back(Letter(QChar(), uivec3(ch, line % gridY, line/gridZ)));
            }
            else
            {
                puzzle.getRefWorkingGrid().push_back(Letter(QChar(46), uivec3(ch, line % gridY, line/gridZ)));
            }
        }
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

        std::vector<Letter> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setX(letterPosition.getX() + j);

            letters.push_back(Letter(letterChar, letterPosition));
        }
        Word word(letters);

        assert(length == wordString.length());
        assert(length == letters.size());

       Clue clue(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, word, clue));
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

        std::vector<Letter> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setY(letterPosition.getY() + j);

            letters.push_back(Letter(letterChar, letterPosition));
        }
        Word word(letters);

        assert(length == wordString.length());
        assert(length == letters.size());

        Clue clue(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, word, clue));
    }

    unsigned int numThrough = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numThrough; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::THROUGH);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt() - 1;
        unsigned int posY = list.takeFirst().toUInt() - 1;
        unsigned int posZ = list.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setZ(letterPosition.getZ() + j);

            letters.push_back(Letter(letterChar, letterPosition));
        }
        Word word(letters);

        assert(length == wordString.length());
        assert(length == letters.size());

        Clue clue(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, word, clue));
    }
}

void PuzzleLoader::readInXWC(Puzzle3D &puzzle, QStringList& linelist)
{
}

FileFormats::FORMAT PuzzleLoader::determineFormat(QStringList& linelist)
{
    return FileFormats::XWC3D;
}
