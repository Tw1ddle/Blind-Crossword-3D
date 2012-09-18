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

bool PuzzleLoader::readInXWC3D(BCrossword3D &puzzle, QStringList& linelist)
{
    static const QString errorTitle = tr("XWC3D crossword load error");

    puzzle.m_CrosswordFileFormat = FileFormats::XWC3D;

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
                    // Here, we may want to load partially solved crossword grids (depending on the crossword type, presumably)
                    puzzle.getRefGrid().push_back(Letter(QChar(Qt::Key_Period), uivec3(x % gridZ , ch, z)));
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

        // Breaks on brackets in clues...

       QStringList clueDecomp = list.takeFirst().split("(");
       Clue clue(clueDecomp.takeFirst());

       QStringList wordComponents = clueDecomp.takeFirst().remove(")").split(QRegExp(","));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
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

        // Breaks on brackets in clues...

       QStringList clueDecomp = list.takeFirst().split("(");
       Clue clue(clueDecomp.takeFirst());

       QStringList wordComponents = clueDecomp.takeFirst().remove(")").split(QRegExp(","));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
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

        // Breaks on brackets in clues...

       QStringList clueDecomp = list.takeFirst().split("(");
       Clue clue(clueDecomp.takeFirst());

       QStringList wordComponents = clueDecomp.takeFirst().remove(")").split(QRegExp(","));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool PuzzleLoader::readInXWC3Dv2(BCrossword3D &puzzle, QStringList &linelist)
{
    QString errorTitle = tr("XWC format crossword load error");

    puzzle.m_CrosswordFileFormat = FileFormats::XWC3D;

    puzzle.m_PuzzleTitle = linelist.takeFirst();
    puzzle.m_AuthorTitle = linelist.takeFirst();
    puzzle.m_PuzzleType = linelist.takeFirst();

    if(puzzle.m_PuzzleTitle.isNull() || puzzle.m_AuthorTitle.isNull() || puzzle.m_PuzzleType.isNull())
    {
        emit(loaderError(errorTitle, tr("Missing author name, author title or puzzle type specifier")));
        return false;
    }

    unsigned int gridX = linelist.takeFirst().toUInt();
    unsigned int gridY = linelist.takeFirst().toUInt();
    unsigned int gridZ = linelist.takeFirst().toUInt();
    puzzle.setDimensions(uivec3(gridX, gridY, gridZ));

    if(gridY <= 0 || gridX <= 0)
    {
        emit(loaderError(errorTitle, tr("Invalid crossword grid dimensions")));
        return false;
    }
}

bool PuzzleLoader::readInXWC(BCrossword3D &puzzle, QStringList& linelist)
{
    QString errorTitle = tr("XWC format crossword load error");

    puzzle.m_CrosswordFileFormat = FileFormats::XWC;

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
                puzzle.getRefGrid().push_back(Letter(QChar(Qt::Key_Period), uivec3(x , ch, 0)));
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

        // Breaks on brackets in clues...

       QStringList clueDecomp = list.takeFirst().split("(");
       Clue clue(clueDecomp.takeFirst());

       QStringList wordComponents = clueDecomp.takeFirst().remove(")").split(QRegExp(","));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
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

        // Breaks on brackets in clues...

       QStringList clueDecomp = list.takeFirst().split("(");
       Clue clue(clueDecomp.takeFirst());

       QStringList wordComponents = clueDecomp.takeFirst().remove(")").split(QRegExp(","));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
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

    QStringList gridlist;
    for(unsigned int i = 0; i < puzzle.getRefGrid().getSize(); i+=puzzle.getRefGrid().getDimensions().getY())
    {
        QString gridString;
        for(unsigned int y = 0; y < puzzle.getRefGrid().getDimensions().getY(); y++)
        {
            if(puzzle.getRefGrid().getLetterAt(i + y).getChar().isNull())
            {
                gridString.push_back(Qt::Key_1);
            }
            else
            {
                gridString.push_back(puzzle.getRefGrid().getLetterAt(i + y).getChar());
            }
        }
        gridlist.push_back(gridString);
    }
    linelist += gridlist;

    unsigned int acrossEntries = 0;
    unsigned int awayEntries = 0;

    for(unsigned int i = 0; i < puzzle.getRefCrosswordEntries().size(); i++)
    {
        if(puzzle.getRefCrosswordEntries().at(i).getDirection() == Directions::ACROSS)
        {
            acrossEntries++;
        }
        if(puzzle.getRefCrosswordEntries().at(i).getDirection() == Directions::AWAY)
        {
            awayEntries++;
        }
    }

    linelist.push_back(QString::number(acrossEntries));
    for(unsigned int i = 0; i < acrossEntries; i++)
    {
        CrosswordEntry3D entry = puzzle.getRefCrosswordEntries().at(i);
        QString entryString = QString::number(entry.getEntryNumber()).append("|")
                .append(QString::number(entry.getStartingPosition().getX() + 1)).append("|")
                .append(QString::number(entry.getStartingPosition().getY() + 1)).append(("|"))
                .append(QString::number(entry.getSolution().size())).append("|")
                .append(entry.getSolution()).append("|")
                .append(entry.getClue().getString()).append(" ")
                .append(entry.getWordComponentsStringWithFormatting());

        linelist.push_back(entryString);
    }

    linelist.push_back(QString::number(awayEntries));
    for(unsigned int i = acrossEntries; i < awayEntries + acrossEntries; i++)
    {
        CrosswordEntry3D entry = puzzle.getRefCrosswordEntries().at(i);
        QString entryString = QString::number(entry.getEntryNumber()).append("|")
                .append(QString::number(entry.getStartingPosition().getX() + 1)).append("|")
                .append(QString::number(entry.getStartingPosition().getY() + 1)).append(("|"))
                .append(QString::number(entry.getSolution().size())).append("|")
                .append(entry.getSolution()).append("|")
                .append(entry.getClue().getString()).append(" ")
                .append(entry.getWordComponentsStringWithFormatting());

        linelist.push_back(entryString);
    }

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

    QStringList gridlist;
    for(unsigned int i = 0; i < puzzle.getRefGrid().getSize(); i+=puzzle.getRefGrid().getDimensions().getY())
    {
        QString gridString;
        for(unsigned int y = 0; y < puzzle.getRefGrid().getDimensions().getY(); y++)
        {
            if(puzzle.getRefGrid().getLetterAt(i + y).getChar().isNull())
            {
                gridString.push_back(Qt::Key_1);
            }
            else
            {
                gridString.push_back(puzzle.getRefGrid().getLetterAt(i + y).getChar());
            }
        }
        gridlist.push_back(gridString);
    }
    linelist += gridlist;

    unsigned int acrossEntries = 0;
    unsigned int awayEntries = 0;
    unsigned int downEntries = 0;

    for(unsigned int i = 0; i < puzzle.getRefCrosswordEntries().size(); i++)
    {
        if(puzzle.getRefCrosswordEntries().at(i).getDirection() == Directions::ACROSS)
        {
            acrossEntries++;
        }
        if(puzzle.getRefCrosswordEntries().at(i).getDirection() == Directions::AWAY)
        {
            awayEntries++;
        }
        if(puzzle.getRefCrosswordEntries().at(i).getDirection() == Directions::DOWN)
        {
            downEntries++;
        }
    }

    linelist.push_back(QString::number(acrossEntries));
    for(unsigned int i = 0; i < acrossEntries; i++)
    {
        CrosswordEntry3D entry = puzzle.getRefCrosswordEntries().at(i);
        QString entryString = QString::number(entry.getEntryNumber()).append("|")
                .append(QString::number(entry.getStartingPosition().getX() + 1)).append("|")
                .append(QString::number(entry.getStartingPosition().getY() + 1)).append(("|"))
                .append(QString::number(entry.getStartingPosition().getZ() + 1)).append(("|"))
                .append(QString::number(entry.getSolution().size())).append("|")
                .append(entry.getSolution()).append("|")
                .append(entry.getClue().getString()).append(" ")
                .append(entry.getWordComponentsStringWithFormatting());

        linelist.push_back(entryString);
    }

    linelist.push_back(QString::number(awayEntries));
    for(unsigned int i = acrossEntries; i < awayEntries + acrossEntries; i++)
    {
        CrosswordEntry3D entry = puzzle.getRefCrosswordEntries().at(i);
        QString entryString = QString::number(entry.getEntryNumber()).append("|")
                .append(QString::number(entry.getStartingPosition().getX() + 1)).append("|")
                .append(QString::number(entry.getStartingPosition().getY() + 1)).append(("|"))
                .append(QString::number(entry.getStartingPosition().getZ() + 1)).append(("|"))
                .append(QString::number(entry.getSolution().size())).append("|")
                .append(entry.getSolution()).append("|")
                .append(entry.getClue().getString()).append(" ")
                .append(entry.getWordComponentsStringWithFormatting());

        linelist.push_back(entryString);
    }

    linelist.push_back(QString::number(downEntries));
    for(unsigned int i = acrossEntries + awayEntries; i < acrossEntries + awayEntries + downEntries; i++)
    {
        CrosswordEntry3D entry = puzzle.getRefCrosswordEntries().at(i);
        QString entryString = QString::number(entry.getEntryNumber()).append("|")
                .append(QString::number(entry.getStartingPosition().getX() + 1)).append("|")
                .append(QString::number(entry.getStartingPosition().getY() + 1)).append(("|"))
                .append(QString::number(entry.getStartingPosition().getZ() + 1)).append(("|"))
                .append(QString::number(entry.getSolution().size())).append("|")
                .append(entry.getSolution()).append("|")
                .append(entry.getClue().getString()).append(" ")
                .append(entry.getWordComponentsStringWithFormatting());

        linelist.push_back(entryString);
    }

    return linelist;
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
