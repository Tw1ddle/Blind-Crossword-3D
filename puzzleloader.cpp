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
        return; // Oh dear
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
    puzzle.m_GridDimensions = uivec3(gridX, gridY, gridZ);

    for(unsigned int z = 0; z < puzzle.m_GridDimensions.getZ(); z++)
    {
        for(unsigned int y = 0; y < puzzle.m_GridDimensions.getY(); y++)
        {
            QString currentLine = linelist.takeFirst();

            for(unsigned int x = 0; x < puzzle.m_GridDimensions.getX(); x++)
            {
                if(currentLine.at(x) == '1')
                {
                    puzzle.m_SolutionLetters.push_back(Letter(QChar(), uivec3(x, y, z)));
                    puzzle.m_WorkingLetters.push_back(Letter(QChar(95u), uivec3(x, y, z)));
                }
                else
                {
                    puzzle.m_SolutionLetters.push_back(Letter(currentLine.at(x), uivec3(x, y, z)));
                    puzzle.m_WorkingLetters.push_back(Letter(QChar(), uivec3(x, y, z)));
                }
            }
        }
    }

    unsigned int numAcross = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numAcross; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::ACROSS);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt();
        unsigned int posY = list.takeFirst().toUInt();
        unsigned int posZ = list.takeFirst().toUInt();
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

        Hint hint(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, startingPosition, word, hint));
    }

    unsigned int numAway = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numAway; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::AWAY);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt();
        unsigned int posY = list.takeFirst().toUInt();
        unsigned int posZ = list.takeFirst().toUInt();
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

        Hint hint(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, startingPosition, word, hint));
    }

    unsigned int numThrough = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numThrough; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::THROUGH);

        unsigned int number = list.takeFirst().toUInt();

        unsigned int posX = list.takeFirst().toUInt();
        unsigned int posY = list.takeFirst().toUInt();
        unsigned int posZ = list.takeFirst().toUInt();
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

        Hint hint(list.takeFirst());

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, startingPosition, word, hint));
    }
}

void PuzzleLoader::readInXWC(Puzzle3D &puzzle, QStringList& linelist)
{
}

FileFormats::FORMAT PuzzleLoader::determineFormat(QStringList& linelist)
{
    return FileFormats::XWC3D;
}
