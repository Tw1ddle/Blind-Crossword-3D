#include "xwc3dloader.h"

#include <algorithm>

bool XWC3DLoader::loadMetaData(CrosswordBase& puzzle, QStringList& linelist)
{
    puzzle.m_FileFormat = linelist.takeFirst();
    puzzle.m_FileFormatVersion = linelist.takeFirst().toFloat();

    if(puzzle.m_FileFormat != FileFormats::XWC3D && puzzle.m_FileFormat != FileFormats::XWCR3D)
    {
        return false;
    }

    puzzle.m_Title = linelist.takeFirst();
    puzzle.m_Authors = linelist.takeFirst();
    puzzle.m_Type = linelist.takeFirst();
    puzzle.m_Notes = linelist.takeFirst();

    if(puzzle.m_Title.isNull() || puzzle.m_Authors.isNull() || puzzle.m_Type.isNull() || puzzle.m_Notes.isNull())
    {
        return false;
    }

    puzzle.m_BackgroundImageFilename = linelist.takeFirst();
    if(!puzzle.loadBackgroundImage(puzzle.m_BackgroundImageFilename))
    {
        //return false;
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

    if(!puzzleCatchPhraseData.empty())
    {
        while(puzzleCatchPhraseData.size() > 0)
        {
            QStringList currentCatchPhraseCoordinateList = puzzleCatchPhraseData.takeFirst().split(",");

            if(currentCatchPhraseCoordinateList.size() == 4)
            {
                unsigned int x = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;
                unsigned int y = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;
                unsigned int z = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;

                QString SVGWorldWideWebColorName = currentCatchPhraseCoordinateList.takeFirst();

                puzzle.m_Highlights.push_back(std::pair<uivec3, QString>(uivec3(x, y, z), SVGWorldWideWebColorName));
            }
        }
    }

    return true;
}

bool XWC3DLoader::loadGrid(CrosswordBase& puzzle, QStringList& linelist)
{
    if(puzzle.getFormat() == FileFormats::XWC3D)
    {
        return loadXWC3DGrid(puzzle, linelist);
    }
    else if(puzzle.getFormat() == FileFormats::XWCR3D)
    {
        return loadXWCR3DGrid(puzzle, linelist);
    }

    return false;
}

bool XWC3DLoader::loadXWC3DGrid(CrosswordBase &puzzle, QStringList &linelist)
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
                    puzzle.getRefGrid().push_back(Letter(QChar(), uivec3(ch, y, z)));
                }
                else
                {
                    puzzle.getRefGrid().push_back(Letter(QChar(currentLine.at(ch)), uivec3(ch, y, z)));
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

bool XWC3DLoader::loadXWCR3DGrid(CrosswordBase &puzzle, QStringList &linelist)
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
                Letter letter(QChar(), uivec3(0, 0, z));
                    for(unsigned int ch = 0; ch < gridX; ch++)
                    {
                    if(currentLine.at(0) == '1')
                    {
                        puzzle.getRefGrid().push_back(letter);
                    }
                    else
                    {
                        letter.setChar(currentLine.at(0));
                        puzzle.getRefGrid().push_back(letter);
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
                        puzzle.getRefGrid().push_back(Letter(QChar(currentLine.at(ch)), uivec3(ch, y, z)));
                    }
                }
            }
        }
    }

    return true;
}

bool XWC3DLoader::loadClues(CrosswordBase& puzzle, QStringList& linelist)
{
    while(linelist.empty() == false)
    {
        QString direction = linelist.takeFirst();

        if(!loadCluesHelper(puzzle, linelist, direction))
        {
            return false;
        }
    }

    return true;
}

bool XWC3DLoader::loadCluesHelper(CrosswordBase &puzzle, QStringList &linelist, QString direc)
{
    QString direction(direc);

    unsigned int numClues = linelist.takeFirst().toUInt();

    if(direction == ClueDirections::SNAKING)
    {
        return loadSnakingClues(puzzle, linelist, numClues);
    }

    for(unsigned int i = 0; i < numClues; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        QString identifier = list.takeFirst();

        QString entry = list.takeFirst();

        QStringList firstPositionList = list.takeFirst().split(",");

        unsigned int posX = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posY = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posZ = firstPositionList.takeFirst().toUInt() - 1;
        uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();
        std::vector<Letter*> letters;
        QString wordString = list.takeFirst();

        if(direction == ClueDirections::ACROSS)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;
                letterPosition.setX(letterPosition.getX() + j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else if(direction == ClueDirections::BACKWARDS)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;
                letterPosition.setX(letterPosition.getX() - j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else if(direction == ClueDirections::AWAY)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;
                letterPosition.setY(letterPosition.getY() + j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else if(direction == ClueDirections::TOWARDS)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;
                letterPosition.setY(letterPosition.getY() - j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else if(direction == ClueDirections::DOWN)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;
                letterPosition.setZ(letterPosition.getZ() + j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else if(direction == ClueDirections::UP)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;
                letterPosition.setZ(letterPosition.getZ() - j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else if(direction == ClueDirections::CLOCKWISE)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;
                letterPosition.setX((letterPosition.getX() + j) % puzzle.getGrid().getDimensions().getX());

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else if(direction == ClueDirections::ANTICLOCKWISE)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;
                letterPosition.setX((letterPosition.getX() - j) % puzzle.getGrid().getDimensions().getX());

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else if(direction == ClueDirections::DIAMETRIC)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                QChar letterChar = wordString.at(j);
                uivec3 letterPosition = startingPosition;

                letterPosition.setY(std::abs(static_cast<long>(letterPosition.getY()) - static_cast<long>(j)));

                if((static_cast<int>(startingPosition.getY()) - static_cast<int>(j)) < 0) //which side of the disc is the diametric letter on?
                {
                    letterPosition.setX((letterPosition.getX() + puzzle.getGrid().getDimensions().getX()/2) % puzzle.getGrid().getDimensions().getX());
                }

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        }
        else
        {
            return false;
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

       puzzle.m_Entries.push_back(CrosswordEntry(direction, identifier, entry, wordString, word, wordComponentLengths, clue));
    }

    return true;
}

bool XWC3DLoader::loadSnakingClues(CrosswordBase& puzzle, QStringList& linelist, unsigned int numsnaking)
{
    QString direction(ClueDirections::SNAKING);

    for(unsigned int i = 0; i < numsnaking; i++)
    {
        QStringList list = linelist.takeFirst().split("|");

        QString identifier = list.takeFirst();

        QStringList entryIndicesStringList = list.takeFirst().split(",", QString::SkipEmptyParts);

        std::vector<unsigned int> entryIndices;
        for(int k = 0; k < entryIndicesStringList.size(); k++)
        {
            entryIndices.push_back(entryIndicesStringList.at(k).toUInt() - 1);
        }

        QString entryString = list.takeFirst();

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

        puzzle.m_Entries.push_back(CrosswordEntry(direction, identifier, entryString, wordString, word, wordComponentLengths, clue, entryIndices));
    }

    return true;
}

bool XWC3DLoader::saveMetaData(CrosswordBase &puzzle, QStringList &linelist)
{
    linelist += puzzle.m_FileFormat;
    linelist += QString::number(puzzle.m_FileFormatVersion);
    linelist += puzzle.m_Title;
    linelist += puzzle.m_Authors;
    linelist += puzzle.m_Type;
    linelist += puzzle.m_Notes;
    linelist += puzzle.m_BackgroundImageFilename;

    linelist += QString::number(puzzle.getRefGrid().getDimensions().getX());
    linelist += QString::number(puzzle.getRefGrid().getDimensions().getY());
    linelist += QString::number(puzzle.getRefGrid().getDimensions().getZ());

    QString highlightCoordinates;

    if(puzzle.m_Highlights.size() == 0)
    {
        highlightCoordinates.push_back("This puzzle has no highlight.");
    }
    else
    {
        for(unsigned int i = 0; i < puzzle.m_Highlights.size(); i++)
        {
            uivec3 coordinate = puzzle.m_Highlights.at(i).first;
            QString color = puzzle.m_Highlights.at(i).second;

            highlightCoordinates.append(QString::number(coordinate.getX() + 1)).append(",")
                    .append(QString::number(coordinate.getY() + 1)).append(",")
                    .append(QString::number(coordinate.getZ() + 1)).append(",")
                    .append(color).append("|");
        }

        highlightCoordinates.truncate(highlightCoordinates.length() - 1);
    }


    linelist += highlightCoordinates;

    return true;
}

bool XWC3DLoader::saveGrid(CrosswordBase &puzzle, QStringList &linelist)
{
    return saveXWC3DGrid(puzzle, linelist);
}

bool XWC3DLoader::saveXWC3DGrid(CrosswordBase &puzzle, QStringList &linelist)
{
    QStringList gridlist;
    for(unsigned int i = 0; i < puzzle.getRefGrid().getSize(); i+=(puzzle.getRefGrid().getDimensions().getX()))
    {
        QString gridString;
        for(unsigned int x = 0; x < puzzle.getRefGrid().getDimensions().getX(); x++)
        {
            if(puzzle.getRefGrid().getLetterAt(i + x)->getChar().isNull())
            {
                gridString.push_back(Qt::Key_1);
            }
            else
            {
                gridString.push_back(puzzle.getRefGrid().getLetterAt(i + x)->getChar());
            }
        }
        gridlist.push_back(gridString);
    }
    linelist += gridlist;

    return true;
}

bool XWC3DLoader::saveClues(CrosswordBase &puzzle, QStringList &linelist)
{
    if(puzzle.getFormat() == FileFormats::XWC3D)
    {
        saveCluesHelper(puzzle, linelist, ClueDirections::ACROSS);
        saveCluesHelper(puzzle, linelist, ClueDirections::BACKWARDS);
        saveCluesHelper(puzzle, linelist, ClueDirections::AWAY);
        saveCluesHelper(puzzle, linelist, ClueDirections::TOWARDS);
        saveCluesHelper(puzzle, linelist, ClueDirections::DOWN);
        saveCluesHelper(puzzle, linelist, ClueDirections::UP);
        saveCluesHelper(puzzle, linelist, ClueDirections::SNAKING);
    }
    else if(puzzle.getFormat() == FileFormats::XWCR3D)
    {
        saveCluesHelper(puzzle, linelist, ClueDirections::CLOCKWISE);
        saveCluesHelper(puzzle, linelist, ClueDirections::ANTICLOCKWISE);
        saveCluesHelper(puzzle, linelist, ClueDirections::DIAMETRIC);
        saveCluesHelper(puzzle, linelist, ClueDirections::DOWN);
        saveCluesHelper(puzzle, linelist, ClueDirections::UP);
        saveCluesHelper(puzzle, linelist, ClueDirections::SNAKING);
    }

    return true;
}

bool XWC3DLoader::saveCluesHelper(CrosswordBase &/*puzzle*/, QStringList &linelist, QString direction)
{
    std::vector<CrosswordEntry> entries;

    // filter the entries by direction and then run the regular save routine. special snaking one for snakes...

    //FIXME! Commented out till I can figure out whether OSX includes this header yet
    //std::copy_if(puzzle.getEntries().begin(), puzzle.getEntries().end(), std::back_inserter(entries), [&](const CrosswordEntry& entry) { return entry.getDirection() == direction; });

    QStringList entrylist;

    entrylist += direction;
    entrylist += QString::number(entries.size());

    for(unsigned int i = 0; i < entries.size(); i++)
    {
        CrosswordEntry entry = entries.at(i);
        QString entryString;

        if(direction != ClueDirections::SNAKING)
        {
            entryString
                    .append(entry.getIdentifier()).append("|")
                    .append(entry.getEntry()).append("|")
                    .append(QString::number(entry.getStartingPosition().getX() + 1)).append(",")
                    .append(QString::number(entry.getStartingPosition().getY() + 1)).append(",")
                    .append(QString::number(entry.getStartingPosition().getZ() + 1)).append("|")
                    .append(QString::number(entry.getSolution().length())).append("|")
                    .append(entry.getSolution()).append("|")
                    .append(entry.getClue()).append("|")
                    .append("(").append(entry.getSolutionComponentLengths()).append(")");
        }
        else
        {
            entryString
                    .append(entry.getIdentifier()).append("|")
                    .append(entry.getEntryPositionsString()).append("|")
                    .append(entry.getEntry()).append("|")
                    .append(QString::number(entry.getSolution().length())).append("|");

            for(int j = 0; j < entry.getSolution().length(); j++)
            {
                uivec3 letterPosition = entry.getGuess().getPositions().at(j);

                entryString
                        .append(QString::number(letterPosition.getX() + 1)).append(",")
                        .append(QString::number(letterPosition.getY() + 1)).append(",")
                        .append(QString::number(letterPosition.getZ() + 1)).append("|");
            }

            entryString
                    .append(entry.getSolution()).append("|")
                    .append(entry.getClue()).append("|")
                    .append("(").append(entry.getSolutionComponentLengths()).append(")");
        }

        entrylist += entryString;
    }

    linelist += entrylist;

    return true;
}
