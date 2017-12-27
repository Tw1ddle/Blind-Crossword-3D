#include "loader/xwc3dloader.h"

#include <algorithm>
#include <cstdlib>

namespace loader {

bool XWC3DLoader::loadMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    puzzle.m_fileFormat = linelist.takeFirst();
    puzzle.m_fileFormatVersion = linelist.takeFirst().toFloat();

    if (puzzle.m_fileFormat != crossword::fileformat::XWC3D &&
            puzzle.m_fileFormat != crossword::fileformat::XWCR3D) {
        return false;
    }

    puzzle.m_title = linelist.takeFirst();
    puzzle.m_authors = linelist.takeFirst();
    puzzle.m_type = linelist.takeFirst();
    puzzle.m_notes = linelist.takeFirst();

    if (puzzle.m_title.isNull() || puzzle.m_authors.isNull() || puzzle.m_type.isNull() ||
            puzzle.m_notes.isNull()) {
        return false;
    }

    puzzle.m_backgroundImageFilename = linelist.takeFirst();

    if (!puzzle.loadBackgroundImage(puzzle.m_backgroundImageFilename)) {
        //return false;
    }

    unsigned int gridX = linelist.takeFirst().toUInt();
    unsigned int gridY = linelist.takeFirst().toUInt();
    unsigned int gridZ = linelist.takeFirst().toUInt();
    puzzle.setDimensions(util::uivec3(gridX, gridY, gridZ));

    if (gridX <= 0 || gridY <= 0 || gridZ <= 0) {
        return false;
    }

    QStringList puzzleCatchPhraseData = linelist.takeFirst().split("|");

    if (!puzzleCatchPhraseData.empty()) {
        while (puzzleCatchPhraseData.size() > 0) {
            QStringList currentCatchPhraseCoordinateList = puzzleCatchPhraseData.takeFirst().split(",");

            if (currentCatchPhraseCoordinateList.size() == 4) {
                unsigned int x = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;
                unsigned int y = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;
                unsigned int z = currentCatchPhraseCoordinateList.takeFirst().toUInt() - 1;

                QString SVGWorldWideWebColorName = currentCatchPhraseCoordinateList.takeFirst();

                puzzle.m_highlights.push_back(std::pair<util::uivec3, QString>(util::uivec3(x, y, z),
                                                                               SVGWorldWideWebColorName));
            }
        }
    }

    return true;
}

bool XWC3DLoader::loadGrid(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    if (puzzle.getFormat() == crossword::fileformat::XWC3D) {
        return loadXWC3DGrid(puzzle, linelist);
    } else if (puzzle.getFormat() == crossword::fileformat::XWCR3D) {
        return loadXWCR3DGrid(puzzle, linelist);
    }

    return false;
}

bool XWC3DLoader::loadXWC3DGrid(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    unsigned int gridX = puzzle.getRefGrid().getDimensions().getX();
    unsigned int gridY = puzzle.getRefGrid().getDimensions().getY();
    unsigned int gridZ = puzzle.getRefGrid().getDimensions().getZ();

    for (unsigned int z = 0; z < gridZ; z++) {
        for (unsigned int y = 0; y < gridY; y++) {
            QString currentLine = linelist.takeFirst();

            for (unsigned int ch = 0; ch < gridX; ch++) {
                if (currentLine.at(ch) == '1') {
                    puzzle.getRefGrid().push_back(crossword::Letter(QChar(), util::uivec3(ch, y, z)));
                } else {
                    puzzle.getRefGrid().push_back(crossword::Letter(QChar(currentLine.at(ch)), util::uivec3(ch, y, z)));
                }
            }
        }
    }

    if (puzzle.getRefGrid().getSize() != (gridX * gridY * gridZ)) {
        return false;
    }

    return true;
}

bool XWC3DLoader::loadXWCR3DGrid(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    unsigned int gridX = puzzle.getRefGrid().getDimensions().getX();
    unsigned int gridY = puzzle.getRefGrid().getDimensions().getY();
    unsigned int gridZ = puzzle.getRefGrid().getDimensions().getZ();

    for (unsigned int z = 0; z < gridZ; z++) {
        for (unsigned int y = 0; y < gridY; y++) {
            QString currentLine = linelist.takeFirst();

            if (y == 0) { // disc hub
                crossword::Letter letter(QChar(), util::uivec3(0, 0, z));

                for (unsigned int ch = 0; ch < gridX; ch++) {
                    if (currentLine.at(0) == '1') {
                        puzzle.getRefGrid().push_back(letter);
                    } else {
                        letter.setChar(currentLine.at(0));
                        puzzle.getRefGrid().push_back(letter);
                    }
                }
            } else {
                for (unsigned int ch = 0; ch < gridX; ch++) {
                    if (currentLine.at(ch) == '1') {
                        puzzle.getRefGrid().push_back(crossword::Letter(QChar(), util::uivec3(ch, y, z)));
                    } else {
                        puzzle.getRefGrid().push_back(crossword::Letter(QChar(currentLine.at(ch)), util::uivec3(ch, y, z)));
                    }
                }
            }
        }
    }

    return true;
}

bool XWC3DLoader::loadClues(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    while (linelist.empty() == false) {
        QString direction = linelist.takeFirst();

        if (!loadCluesHelper(puzzle, linelist, direction)) {
            return false;
        }
    }

    return true;
}

bool XWC3DLoader::loadCluesHelper(crossword::CrosswordBase& puzzle, QStringList& linelist,
                                  QString direc)
{
    QString direction(direc);

    unsigned int numClues = linelist.takeFirst().toUInt();

    if (direction == crossword::cluedirections::SNAKING) {
        return loadSnakingClues(puzzle, linelist, numClues);
    }

    for (unsigned int i = 0; i < numClues; i++) {
        QStringList list = linelist.takeFirst().split("|");

        QString identifier = list.takeFirst();

        QString entry = list.takeFirst();

        QStringList firstPositionList = list.takeFirst().split(",");

        unsigned int posX = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posY = firstPositionList.takeFirst().toUInt() - 1;
        unsigned int posZ = firstPositionList.takeFirst().toUInt() - 1;
        util::uivec3 startingPosition(posX, posY, posZ);

        unsigned int length = list.takeFirst().toUInt();
        std::vector<crossword::Letter*> letters;
        QString wordString = list.takeFirst();

        if (direction == crossword::cluedirections::ACROSS) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;
                letterPosition.setX(letterPosition.getX() + j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else if (direction == crossword::cluedirections::BACKWARDS) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;
                letterPosition.setX(letterPosition.getX() - j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else if (direction == crossword::cluedirections::AWAY) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;
                letterPosition.setY(letterPosition.getY() + j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else if (direction == crossword::cluedirections::TOWARDS) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;
                letterPosition.setY(letterPosition.getY() - j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else if (direction == crossword::cluedirections::DOWN) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;
                letterPosition.setZ(letterPosition.getZ() + j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else if (direction == crossword::cluedirections::UP) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;
                letterPosition.setZ(letterPosition.getZ() - j);

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else if (direction == crossword::cluedirections::CLOCKWISE) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;
                letterPosition.setX((letterPosition.getX() + j) % puzzle.getGrid().getDimensions().getX());

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else if (direction == crossword::cluedirections::ANTICLOCKWISE) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;
                letterPosition.setX((letterPosition.getX() - j) % puzzle.getGrid().getDimensions().getX());

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else if (direction == crossword::cluedirections::DIAMETRIC) {
            for (unsigned int j = 0; j < length; j++) {
                util::uivec3 letterPosition = startingPosition;

                letterPosition.setY(std::abs(static_cast<long>(letterPosition.getY()) - static_cast<long>(j)));

                if ((static_cast<int>(startingPosition.getY()) - static_cast<int>(j)) <
                        0) { //which side of the disc is the diametric letter on?
                    letterPosition.setX((letterPosition.getX() + puzzle.getGrid().getDimensions().getX() / 2) %
                                        puzzle.getGrid().getDimensions().getX());
                }

                letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
            }
        } else {
            return false;
        }

        crossword::Word word(letters);

        if (length != wordString.length() || length != letters.size()) {
            return false;
        }

        QString clue(list.takeFirst());

        QString clueDecomp = list.takeFirst().remove("(").remove(")");
        QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;

        while (!wordComponents.isEmpty()) {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_entries.push_back(crossword::CrosswordEntry(direction, identifier, entry, wordString, word,
                                                             wordComponentLengths, clue));
    }

    return true;
}

bool XWC3DLoader::loadSnakingClues(crossword::CrosswordBase& puzzle, QStringList& linelist,
                                   unsigned int numsnaking)
{
    QString direction(crossword::cluedirections::SNAKING);

    for (unsigned int i = 0; i < numsnaking; i++) {
        QStringList list = linelist.takeFirst().split("|");

        QString identifier = list.takeFirst();

        QStringList entryIndicesStringList = list.takeFirst().split(",", QString::SkipEmptyParts);

        std::vector<unsigned int> entryIndices;

        for (int k = 0; k < entryIndicesStringList.size(); k++) {
            entryIndices.push_back(entryIndicesStringList.at(k).toUInt() - 1);
        }

        QString entryString = list.takeFirst();

        std::vector<util::uivec3> letterPositions;
        unsigned int length = list.takeFirst().toUInt();

        for (unsigned int k = 0; k < length; k++) {
            QStringList letterPosition = list.takeFirst().split(",");
            util::uivec3 letterPositionVec = util::uivec3(letterPosition.takeLast().toUInt() - 1,
                                                          letterPosition.takeLast().toUInt() - 1, letterPosition.takeLast().toUInt() - 1);
            letterPositions.push_back(letterPositionVec);
        }

        std::vector<crossword::Letter*> letters;

        QString wordString = list.takeFirst();

        for (unsigned int l = 0; l < length; l++) {
            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPositions.at(l))));
        }

        crossword::Word word(letters);

        if (length != wordString.length() || length != letters.size()) {
            return false;
        }

        QString clue(list.takeFirst());

        QString clueDecomp = list.takeFirst().remove("(").remove(")");
        QStringList wordComponents = clueDecomp.split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;

        while (!wordComponents.isEmpty()) {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_entries.push_back(crossword::CrosswordEntry(direction, identifier, entryString, wordString,
                                                             word,
                                                             wordComponentLengths, clue, entryIndices));
    }

    return true;
}

bool XWC3DLoader::saveMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    linelist += puzzle.m_fileFormat;
    linelist += QString::number(puzzle.m_fileFormatVersion);
    linelist += puzzle.m_title;
    linelist += puzzle.m_authors;
    linelist += puzzle.m_type;
    linelist += puzzle.m_notes;
    linelist += puzzle.m_backgroundImageFilename;

    linelist += QString::number(puzzle.getRefGrid().getDimensions().getX());
    linelist += QString::number(puzzle.getRefGrid().getDimensions().getY());
    linelist += QString::number(puzzle.getRefGrid().getDimensions().getZ());

    QString highlightCoordinates;

    if (puzzle.m_highlights.size() == 0) {
        highlightCoordinates.push_back("This puzzle has no highlight.");
    } else {
        for (unsigned int i = 0; i < puzzle.m_highlights.size(); i++) {
            util::uivec3 coordinate = puzzle.m_highlights.at(i).first;
            QString color = puzzle.m_highlights.at(i).second;

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

bool XWC3DLoader::saveGrid(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    return saveXWC3DGrid(puzzle, linelist);
}

bool XWC3DLoader::saveXWC3DGrid(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    QStringList gridlist;

    for (unsigned int i = 0; i < puzzle.getRefGrid().getSize();
            i += (puzzle.getRefGrid().getDimensions().getX())) {
        QString gridString;

        for (unsigned int x = 0; x < puzzle.getRefGrid().getDimensions().getX(); x++) {
            if (puzzle.getRefGrid().getLetterAt(i + x)->getChar().isNull()) {
                gridString.push_back(Qt::Key_1);
            } else {
                gridString.push_back(puzzle.getRefGrid().getLetterAt(i + x)->getChar());
            }
        }

        gridlist.push_back(gridString);
    }

    linelist += gridlist;

    return true;
}

bool XWC3DLoader::saveClues(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    if (puzzle.getFormat() == crossword::fileformat::XWC3D) {
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::ACROSS);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::BACKWARDS);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::AWAY);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::TOWARDS);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::DOWN);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::UP);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::SNAKING);
    } else if (puzzle.getFormat() == crossword::fileformat::XWCR3D) {
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::CLOCKWISE);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::ANTICLOCKWISE);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::DIAMETRIC);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::DOWN);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::UP);
        saveCluesHelper(puzzle, linelist, crossword::cluedirections::SNAKING);
    }

    return true;
}

bool XWC3DLoader::saveCluesHelper(crossword::CrosswordBase& /*puzzle*/, QStringList& linelist,
                                  QString direction)
{
    std::vector<crossword::CrosswordEntry> entries;

    // filter the entries by direction and then run the regular save routine. special snaking one for snakes...

    //FIXME! Commented out till I can figure out whether OSX includes this header yet
    //std::copy_if(puzzle.getEntries().begin(), puzzle.getEntries().end(), std::back_inserter(entries), [&](const CrosswordEntry& entry) { return entry.getDirection() == direction; });

    QStringList entrylist;

    entrylist += direction;
    entrylist += QString::number(entries.size());

    for (unsigned int i = 0; i < entries.size(); i++) {
        crossword::CrosswordEntry entry = entries.at(i);
        QString entryString;

        if (direction != crossword::cluedirections::SNAKING) {
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
        } else {
            entryString
            .append(entry.getIdentifier()).append("|")
            .append(entry.getEntryPositionsString()).append("|")
            .append(entry.getEntry()).append("|")
            .append(QString::number(entry.getSolution().length())).append("|");

            for (int j = 0; j < entry.getSolution().length(); j++) {
                util::uivec3 letterPosition = entry.getGuess().getPositions().at(j);

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

}
