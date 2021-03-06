#include "loader/xwcloader.h"

namespace loader {

bool XWCLoader::loadMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    puzzle.m_fileFormat = crossword::fileformat::XWC;

    puzzle.m_title = linelist.takeFirst();
    puzzle.m_authors = linelist.takeFirst();
    puzzle.m_type = linelist.takeFirst();

    if (puzzle.m_title.isNull() || puzzle.m_authors.isNull() || puzzle.m_type.isNull()) {
        return false;
    }

    unsigned int gridY = linelist.takeFirst().toUInt();
    unsigned int gridX = linelist.takeFirst().toUInt();
    puzzle.setDimensions(util::uivec3(gridX, gridY, 1));

    if (gridY <= 0 || gridX <= 0) {
        return false;
    }

    return true;
}

bool XWCLoader::loadGrid(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    unsigned int gridX = puzzle.getRefGrid().getDimensions().getX();
    unsigned int gridY = puzzle.getRefGrid().getDimensions().getY();

    for (unsigned int x = 0; x < gridX; x++) {
        QString currentLine = linelist.takeFirst();

        for (unsigned int ch = 0; ch < gridY; ch++) {
            if (currentLine.at(ch) == '1') {
                puzzle.getRefGrid().push_back(crossword::Letter(QChar(), util::uivec3(ch, x, 0)));
            } else {
                puzzle.getRefGrid().push_back(crossword::Letter(QChar(Qt::Key_Period), util::uivec3(ch, x, 0)));
            }
        }
    }

    if (puzzle.getRefGrid().getSize() != (gridX * gridY)) {
        return false;
    }

    return true;
}

bool XWCLoader::loadClues(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    if (!loadAcrossClues(puzzle, linelist)) {
        return false;
    }

    if (!loadAwayClues(puzzle, linelist)) {
        return false;
    }

    return true;
}

bool XWCLoader::loadAcrossClues(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    unsigned int numAcross = linelist.takeFirst().toUInt();

    for (unsigned int i = 0; i < numAcross; i++) {
        QStringList list = linelist.takeFirst().split("|");

        QString direction(crossword::cluedirections::ACROSS);

        QString identifier = "0";

        QString number = list.takeFirst();

        unsigned int posY = list.takeFirst().toUInt() - 1;
        unsigned int posX = list.takeFirst().toUInt() - 1;

        if (posX > puzzle.getRefGrid().getDimensions().getX() ||
                posY > puzzle.getRefGrid().getDimensions().getY()) {
            return false;
        }

        util::uivec3 startingPosition(posX, posY, 0);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<crossword::Letter*> letters;
        QString wordString = list.takeFirst();

        for (unsigned int j = 0; j < length; j++) {
            util::uivec3 letterPosition = startingPosition;
            letterPosition.setX(letterPosition.getX() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
        }

        crossword::Word word(letters);

        if (length != wordString.length() || length != letters.size()) {
            return false;
        }

        // Breaks on brackets in clues
        QStringList clueDecomp = list.takeFirst().split("(");
        QString clue(clueDecomp.takeFirst());

        QStringList wordComponents = clueDecomp.takeFirst().remove(")").split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;

        while (!wordComponents.isEmpty()) {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_entries.push_back(crossword::CrosswordEntry(direction, identifier, number, wordString,
                                                             word,
                                                             wordComponentLengths, clue));
    }

    return true;
}


bool XWCLoader::loadAwayClues(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    unsigned int numAway = linelist.takeFirst().toUInt();

    for (unsigned int i = 0; i < numAway; i++) {
        QStringList list = linelist.takeFirst().split("|");

        QString direction(crossword::cluedirections::AWAY);

        QString identifier = "0"; // unused

        QString number = list.takeFirst();

        unsigned int posY = list.takeFirst().toUInt() - 1;
        unsigned int posX = list.takeFirst().toUInt() - 1;
        util::uivec3 startingPosition(posX, posY, 0);

        unsigned int length = list.takeFirst().toUInt();

        std::vector<crossword::Letter*> letters;
        QString wordString = list.takeFirst();

        for (unsigned int j = 0; j < length; j++) {
            util::uivec3 letterPosition = startingPosition;
            letterPosition.setY(letterPosition.getY() + j);

            letters.push_back(puzzle.getRefGrid().getRefLetterAt(puzzle.toGridIndex(letterPosition)));
        }

        crossword::Word word(letters);

        if (length != wordString.length() || length != letters.size()) {
            return false;
        }

        // Breaks on brackets in clues...

        QStringList clueDecomp = list.takeFirst().split("(");
        QString clue(clueDecomp.takeFirst());

        QStringList wordComponents = clueDecomp.takeFirst().remove(")").split(QRegExp("[,-]"));

        std::vector<unsigned int> wordComponentLengths;

        while (!wordComponents.isEmpty()) {
            wordComponentLengths.push_back(wordComponents.takeFirst().toUInt());
        }

        puzzle.m_entries.push_back(crossword::CrosswordEntry(direction, identifier, number, wordString,
                                                             word,
                                                             wordComponentLengths, clue));
    }

    return true;
}

bool XWCLoader::saveMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    linelist.push_back(puzzle.m_title);
    linelist.push_back(puzzle.m_authors);
    linelist.push_back(puzzle.m_type);
    linelist.push_back(QString::number(puzzle.getRefGrid().getDimensions().getX()));
    linelist.push_back(QString::number(puzzle.getRefGrid().getDimensions().getY()));

    return true;
}

bool XWCLoader::saveGrid(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    QStringList gridlist;

    for (unsigned int i = 0; i < puzzle.getRefGrid().getSize();
            i += puzzle.getRefGrid().getDimensions().getY()) {
        QString gridString;

        for (unsigned int y = 0; y < puzzle.getRefGrid().getDimensions().getY(); y++) {
            if (puzzle.getRefGrid().getLetterAt(i + y)->getChar().isNull()) {
                gridString.push_back(Qt::Key_1);
            } else {
                gridString.push_back(puzzle.getRefGrid().getLetterAt(i + y)->getChar());
            }
        }

        gridlist.push_back(gridString);
    }

    linelist += gridlist;

    return true;
}

bool XWCLoader::saveClues(crossword::CrosswordBase& puzzle, QStringList& linelist)
{
    unsigned int acrossEntries = 0;
    unsigned int awayEntries = 0;

    for (unsigned int i = 0; i < puzzle.getRefEntries().size(); i++) {
        if (puzzle.getRefEntries().at(i).getDirection() == crossword::cluedirections::ACROSS) {
            acrossEntries++;
        }

        if (puzzle.getRefEntries().at(i).getDirection() == crossword::cluedirections::AWAY) {
            awayEntries++;
        }
    }

    linelist.push_back(QString::number(acrossEntries));

    for (unsigned int i = 0; i < acrossEntries; i++) {
        crossword::CrosswordEntry entry = puzzle.getRefEntries().at(i);
        QString entryString = entry.getEntry().append("|")
                              .append(QString::number(entry.getStartingPosition().getX() + 1)).append("|")
                              .append(QString::number(entry.getStartingPosition().getY() + 1)).append(("|"))
                              .append(QString::number(entry.getSolution().size())).append("|")
                              .append(entry.getSolution()).append("|")
                              .append(entry.getClue()).append(" ")
                              .append(entry.getSolutionComponentLengths().prepend("(").append(")"));

        linelist.push_back(entryString);
    }

    linelist.push_back(QString::number(awayEntries));

    for (unsigned int i = acrossEntries; i < awayEntries + acrossEntries; i++) {
        crossword::CrosswordEntry entry = puzzle.getRefEntries().at(i);
        QString entryString = entry.getEntry().append("|")
                              .append(QString::number(entry.getStartingPosition().getX() + 1)).append("|")
                              .append(QString::number(entry.getStartingPosition().getY() + 1)).append(("|"))
                              .append(QString::number(entry.getSolution().size())).append("|")
                              .append(entry.getSolution()).append("|")
                              .append(entry.getClue()).append(" ")
                              .append(entry.getSolutionComponentLengths().prepend("(").append(")"));

        linelist.push_back(entryString);
    }

    return true;
}

}
