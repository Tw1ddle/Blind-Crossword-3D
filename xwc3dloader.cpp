#include "xwc3dloader.h"

bool XWC3DLoader::loadMetaData(BCrossword3D& puzzle, QStringList& linelist)
{
    puzzle.m_CrosswordFileFormat = FileFormats::XWC3D;

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

    for(unsigned int i = 0; i < puzzleCatchPhraseData.size(); i++)
    {
        QStringList currentCatchPhraseCoordinateList = puzzleCatchPhraseData.takeFirst().split(",");

        for(unsigned int j = 0; j < currentCatchPhraseCoordinateList.size(); j++)
        {
            unsigned int x = currentCatchPhraseCoordinateList.takeFirst().toUInt();
            unsigned int y = currentCatchPhraseCoordinateList.takeFirst().toUInt();
            unsigned int z = currentCatchPhraseCoordinateList.takeFirst().toUInt();
            puzzle.m_ThemePhraseCoordinates.push_back(uivec3(x, y, z));
        }
    }

    return true;
}

bool XWC3DLoader::loadGrid(BCrossword3D& puzzle, QStringList& linelist)
{
    unsigned int gridX = puzzle.getRefGrid().getDimensions().getX();
    unsigned int gridY = puzzle.getRefGrid().getDimensions().getY();
    unsigned int gridZ = puzzle.getRefGrid().getDimensions().getZ();

    for(unsigned int z = 0; z < gridZ; z++)
    {
        for(unsigned int y = 0; y < gridY; y++)
        {
            QString currentLine = linelist.takeFirst();

            for(unsigned int ch = 0; ch < gridX; ch++)
            {
                if(currentLine.at(ch) == '1')
                {
                    puzzle.getRefGrid().push_back(Letter(QChar(), uivec3(ch , y, z)));
                }
                else
                {
                    // Here, we may want to load partially solved crossword grids (depending on the crossword type, presumably)
                    puzzle.getRefGrid().push_back(Letter(QChar(Qt::Key_Period), uivec3(ch, y, z)));
                }
            }
        }
    }

    if(puzzle.getRefGrid().getSize() != (gridX * gridY * gridZ))
    {
        return false;
    }

    return true;
}

bool XWC3DLoader::loadClues(BCrossword3D& puzzle, QStringList& linelist)
{
    linelist.takeFirst();
    if(!loadAcrossClues(puzzle, linelist))
    {
        return false;
    }

    linelist.takeFirst();
    if(!loadAwayClues(puzzle, linelist))
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
    if(!loadWindingClues(puzzle, linelist))
    {
        return false;
    }

    return true;
}

bool XWC3DLoader::loadAcrossClues(BCrossword3D& puzzle, QStringList& linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::ACROSS);

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
            letterPosition.setX(letterPosition.getX() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

       Clue clue(list.takeFirst());

       QString clueDecomp = list.takeFirst().remove("(").remove(")");
       QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

       std::vector<unsigned int> wordComponentLengths;
       while(!wordComponents.isEmpty())
       {
           wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
       }

       puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWC3DLoader::loadAwayClues(BCrossword3D& puzzle, QStringList& linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::AWAY);

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
            letterPosition.setY(letterPosition.getY() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

        Clue clue(list.takeFirst());

        QString clueDecomp = list.takeFirst().remove("(").remove(")");
        QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;
        while(!wordComponents.isEmpty())
        {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWC3DLoader::loadDownClues(BCrossword3D& puzzle, QStringList& linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::DOWN);

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

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

        Clue clue(list.takeFirst());

        QString clueDecomp = list.takeFirst().remove("(").remove(")");
        QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;
        while(!wordComponents.isEmpty())
        {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWC3DLoader::loadUpClues(BCrossword3D& puzzle, QStringList& linelist)
{
    unsigned int numClues = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::DOWN);

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

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPosition));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

        Clue clue(list.takeFirst());

        QString clueDecomp = list.takeFirst().remove("(").remove(")");
        QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;
        while(!wordComponents.isEmpty())
        {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, number, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWC3DLoader::loadWindingClues(BCrossword3D& puzzle, QStringList& linelist)
{
    unsigned int numWinding = linelist.takeFirst().toUInt();

    for(unsigned int i = 0; i < numWinding; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        Direction direction(Directions::WINDING);

        QStringList directionsList = list.takeFirst().split(",");

        QString entryString;
        for(unsigned int j = 0; j < directionsList.size(); j++)
        {
            entryString.append(directionsList.at(j));
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
            letters.push_back(puzzle.getRefGrid().getRefLetterAt(letterPositions.at(l)));
        }
        Word word(letters);

        if(length != wordString.length() || length != letters.size())
        {
            return false;
        }

        Clue clue(list.takeFirst());

        QString clueDecomp = list.takeFirst().remove("(").remove(")");
        QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;
        while(!wordComponents.isEmpty())
        {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_CrosswordEntries.push_back(CrosswordEntry3D(direction, entryString, wordString, word, wordComponentLengths, clue));
    }

    return true;
}
