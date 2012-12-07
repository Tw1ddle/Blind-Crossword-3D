#include "xwcr3dloader.h"

#include <math.h>

XWCR3DLoader::XWCR3DLoader()
{
}

bool XWCR3DLoader::loadMetaData(PuzzleBase& puzzle, QStringList& linelist)
{
    puzzle.m_CrosswordFileFormat = linelist.takeFirst();
    puzzle.m_FileFormatVersion = linelist.takeFirst().toUInt();

    if(puzzle.m_CrosswordFileFormat != FileFormats::XWC3D)
    {
        return false;
    }

    puzzle.m_PuzzleTitle = linelist.takeFirst();
    puzzle.m_AuthorTitle = linelist.takeFirst();
    puzzle.m_PuzzleType = linelist.takeFirst();
    puzzle.m_PuzzleNotes = linelist.takeFirst();

    if(puzzle.m_PuzzleTitle.isNull() || puzzle.m_AuthorTitle.isNull() || puzzle.m_PuzzleType.isNull() || puzzle.m_PuzzleNotes.isNull())
    {
        return false;
    }

    if(!puzzle.loadBackgroundImage(linelist.takeFirst()))
    {
        return false;
    }

    unsigned int gridX = linelist.takeFirst().toUInt();
    unsigned int gridY = linelist.takeFirst().toUInt();
    unsigned int gridZ = linelist.takeFirst().toUInt();
    puzzle.setDimensions(uivec3(gridX, gridY, gridZ));

    if(gridX <= 0 || gridY <= 0 || gridZ <= 0)
    {
        return false;
    }

    QStringList puzzleCatchPhraseData = linelist.takeFirst().split("|");
    puzzle.m_PuzzleThemePhrase = puzzleCatchPhraseData.takeFirst();

    while(puzzleCatchPhraseData.size() > 0)
    {
        QStringList currentCatchPhraseCoordinateList = puzzleCatchPhraseData.takeFirst().split(",");

        unsigned int x = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;
        unsigned int y = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;
        unsigned int z = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;
        puzzle.m_ThemePhraseCoordinates.push_back(uivec3(x, y, z));
    }

    return true;
}

bool XWCR3DLoader::loadGrid(PuzzleBase &puzzle, QStringList &linelist)
{
    unsigned int gridX = puzzle.getRefGrid().getDimensions().getX();
    unsigned int gridY = puzzle.getRefGrid().getDimensions().getY();
    unsigned int gridZ = puzzle.getRefGrid().getDimensions().getZ();

    for(unsigned int z = 0; z < gridZ; z++)
    {
        for(unsigned int y = 0; y < gridY; y++)
        {
            QString currentLine = linelist.takeFirst();

            if(y == 0) // disc hub
            {
                if(currentLine.at(0) == '1')
                {
                    puzzle.getRefGrid().push_back(Letter(QChar(), uivec3(0, 0, z)));
                }
                else
                {
                    if(puzzle.getPuzzleType() == CrosswordTypes::WITH_ANSWERS_UNSTARTED)
                    {
                        puzzle.getRefGrid().push_back(Letter(QChar(Qt::Key_Period), uivec3(0, 0, z)));
                    }
                    else
                    {
                        puzzle.getRefGrid().push_back(Letter(QChar(currentLine.at(0)), uivec3(0, 0, z)));
                    }
                }
            }
            else
            {
                for(unsigned int ch = 0; ch < gridX; ch++)
                {
                    if(currentLine.at(ch) == '1')
                    {
                        puzzle.getRefGrid().push_back(Letter(QChar(), uivec3(ch, y, z)));
                    }
                    else
                    {
                        if(puzzle.getPuzzleType() == CrosswordTypes::WITH_ANSWERS_UNSTARTED)
                        {
                            puzzle.getRefGrid().push_back(Letter(QChar(Qt::Key_Period), uivec3(ch, y, z)));
                        }
                        else
                        {
                            puzzle.getRefGrid().push_back(Letter(QChar(currentLine.at(ch)), uivec3(ch, y, z)));
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool XWCR3DLoader::loadClues(PuzzleBase &puzzle, QStringList &linelist)
{
    linelist.takeFirst();
    if(!loadClockwiseClues(puzzle, linelist))
    {
        return false;
    }

    linelist.takeFirst();
    if(!loadAntiClockwiseClues(puzzle, linelist))
    {
        return false;
    }

    linelist.takeFirst();
    if(!loadDiametricClues(puzzle, linelist))
    {
        return false;
    }

    linelist.takeFirst();
    if(!loadDownClues(puzzle, linelist))
    {
        return false;
    }

    linelist.takeFirst();
    if(!loadUpClues(puzzle, linelist))
    {
        return false;
    }

    linelist.takeFirst();
    if(!loadSnakingClues(puzzle, linelist))
    {
        return false;
    }

    return true;
}

bool XWCR3DLoader::saveMetaData(PuzzleBase &puzzle, QStringList &linelist)
{
    return false;
}

bool XWCR3DLoader::saveGrid(PuzzleBase &puzzle, QStringList &linelist)
{
    return false;
}

bool XWCR3DLoader::saveClues(PuzzleBase &puzzle, QStringList &linelist)
{
    return false;
}

bool XWCR3DLoader::loadClockwiseClues(PuzzleBase &puzzle, QStringList &linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::CLOCKWISE);

        unsigned int identifier = list.takeFirst().toUInt();

        QString number = list.takeFirst();

        QStringList firstPositionList = list.takeFirst().split(",");

        unsigned int posX = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posY = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posZ = firstPositionList.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setX((letterPosition.getX() + j) % puzzle.getGrid().getDimensions().getX());

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

       QString clue(list.takeFirst());

       QString clueDecomp = list.takeFirst().remove("(").remove(")");
       QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, identifier, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWCR3DLoader::loadAntiClockwiseClues(PuzzleBase &puzzle, QStringList &linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::ANTICLOCKWISE);

        unsigned int identifier = list.takeFirst().toUInt();

        QString number = list.takeFirst();

        QStringList firstPositionList = list.takeFirst().split(",");

        unsigned int posX = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posY = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posZ = firstPositionList.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setX((letterPosition.getX() - j) % puzzle.getGrid().getDimensions().getX());

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

       QString clue(list.takeFirst());

       QString clueDecomp = list.takeFirst().remove("(").remove(")");
       QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, identifier, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWCR3DLoader::loadDiametricClues(PuzzleBase &puzzle, QStringList &linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::DIAMETRIC);

        unsigned int identifier = list.takeFirst().toUInt();

        QString number = list.takeFirst();

        QStringList firstPositionList = list.takeFirst().split(",");

        unsigned int posX = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posY = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posZ = firstPositionList.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;

            letterPosition.setY(std::abs((double)letterPosition.getY() - (double)j));

            if(((int)startingPosition.getY()) - j < 0)
            {
                letterPosition.setX((letterPosition.getX() + puzzle.getGrid().getDimensions().getX()/2) % puzzle.getGrid().getDimensions().getX());
            }

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

       QString clue(list.takeFirst());

       QString clueDecomp = list.takeFirst().remove("(").remove(")");
       QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, identifier, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWCR3DLoader::loadDownClues(PuzzleBase &puzzle, QStringList &linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::DOWN);

        unsigned int identifier = list.takeFirst().toUInt();

        QString number = list.takeFirst();

        QStringList firstPositionList = list.takeFirst().split(",");

        unsigned int posX = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posY = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posZ = firstPositionList.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setZ(letterPosition.getZ() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

       QString clue(list.takeFirst());

       QString clueDecomp = list.takeFirst().remove("(").remove(")");
       QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, identifier, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWCR3DLoader::loadUpClues(PuzzleBase &puzzle, QStringList &linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::UP);

        unsigned int identifier = list.takeFirst().toUInt();

        QString number = list.takeFirst();

        QStringList firstPositionList = list.takeFirst().split(",");

        unsigned int posX = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posY = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posZ = firstPositionList.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();
        for(unsigned int j = 0; j < length; j++)
        {
            QChar letterChar = wordString.at(j);
            uivec3 letterPosition = startingPosition;
            letterPosition.setZ(letterPosition.getZ() - j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

       QString clue(list.takeFirst());

       QString clueDecomp = list.takeFirst().remove("(").remove(")");
       QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, identifier, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWCR3DLoader::loadSnakingClues(PuzzleBase &puzzle, QStringList &linelist)
{
    unsigned int numsnaking = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numsnaking; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::SNAKING);

        unsigned int identifier = list.takeFirst().toUInt();

        QStringList DirectionsList = list.takeFirst().split(",");

        QString entryString;
        for(unsigned int j = 0; j < DirectionsList.size(); j++)
        {
            entryString.append(DirectionsList.at(j));
            entryString.append(" ");
        }

        std::vector<uivec3> letterPositions;
        unsigned int length = list.takeFirst().toUInt();
        for(unsigned int k = 0; k < length; k++)
        {
            QStringList letterPosition = list.takeFirst().split(",");
            uivec3 letterPositionVec = uivec3(letterPosition.takeLast().toUInt() - 1, letterPosition.takeLast().toUInt() - 1, letterPosition.takeLast().toUInt() - 1);
            letterPositions.push_back(letterPositionVec);
        }

        std::vector<Letter*> letters;

        QString wordString = list.takeFirst();
        for(unsigned int l = 0; l < length; l++)
        {
            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPositions.at(l))));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

        QString clue(list.takeFirst());

        QString clueDecomp = list.takeFirst().remove("(").remove(")");
        QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;
        while(!wordComponents.isEmpty())
        {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, identifier, entryString, wordString, word, wordComponentLengths, clue));
    }

    return true;
}
