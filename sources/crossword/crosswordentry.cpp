#include "crosswordentry.h"

#include <QStringList>

#include <assert.h>

CrosswordEntry::CrosswordEntry(QString direction, QString identifier, QString entry,
                               QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, QString clue,
                               std::vector<unsigned int> entryIndices)
{
    m_id = identifier;
    m_wordDirection = direction;
    m_entryString = entry;
    m_solution = solution;
    m_guess = guess;
    m_solutionComponentLengths = wordComponentLengths;
    m_clue = clue;
    m_entryIndices = entryIndices;
}

uivec3 CrosswordEntry::getStartingPosition() const
{
    return m_guess.getPositions().at(0);
}

std::vector<uivec3> CrosswordEntry::getEntryPositions() const
{
    std::vector<uivec3> entryPositions;

    if (m_entryIndices.empty()) {
        entryPositions.push_back(m_guess.getPositions().at(0));
        return entryPositions;
    } else {
        for (unsigned int i = 0; i < m_entryIndices.size(); i++) {
            entryPositions.push_back(getGuess().getPositions().at(m_entryIndices.at(i)));
        }

        return entryPositions;
    }
}

QString CrosswordEntry::getEntryPositionsString() const
{
    QString entryIndices;

    for (unsigned int i = 0; i < m_entryIndices.size(); i++) {
        entryIndices.append(QString::number(m_entryIndices.at(i) +
                                            1)).append(","); // + 1, we use this for exporting to xwc3d files
    }

    entryIndices.truncate(entryIndices.size() - 1);

    return entryIndices;
}

QString CrosswordEntry::getSolution() const
{
    return m_solution;
}

QString CrosswordEntry::getClue() const
{
    return m_clue;
}

Word CrosswordEntry::getGuess() const
{
    return m_guess;
}

void CrosswordEntry::setGuess(QString word)
{
    assert(m_guess.getLength() == m_solution.length());

    m_guess.setString(word);
}

QString CrosswordEntry::getSolutionComponentLengths() const
{
    QString comp;

    for (unsigned int i = 0; i < m_solutionComponentLengths.size(); i++) {
        comp.append(QString::number(m_solutionComponentLengths.at(i)));

        if (i != m_solutionComponentLengths.size() - 1) {
            comp.append(",");
        }
    }

    return comp.trimmed();
}

QString CrosswordEntry::getIdentifier() const
{
    return m_id;
}

void CrosswordEntry::resetGuess()
{
    m_guess.setString(QString(m_guess.getString().size(), QChar(46)));
}

bool CrosswordEntry::isGuessCorrect() const
{
    if (m_guess.getString() == m_solution) {
        return true;
    } else {
        return false;
    }
}

bool CrosswordEntry::intersectsWord(Word* word) const
{
    return m_guess.intersectsWord(word);
}

QString CrosswordEntry::getDirection() const
{
    return m_wordDirection;
}

QString CrosswordEntry::getEntry() const
{
    return m_entryString;
}

std::vector<std::pair<unsigned int, uivec3> > CrosswordEntry::getWordEntryStartingPositionPairs()
const
{
    std::vector<std::pair<unsigned int, uivec3> > pairs;

    QStringList entries = m_entryString.split(" ", QString::SkipEmptyParts);
    std::vector<unsigned int> entryNumbers;

    for (int i = 0; i < entries.size(); i++) {
        bool converted = false;
        unsigned int entryNumber = entries.at(i).toUInt(&converted);

        if (converted) {
            entryNumbers.push_back(entryNumber);
        }
    }

    if (getEntryPositions().size() ==
            entryNumbers.size()) { // this should only ever be the case if we have a line like "Stage 2 Anagram" as the entry name
        for (unsigned int k = 0; k < entryNumbers.size(); k++) {
            pairs.push_back(std::pair<unsigned int, uivec3>(entryNumbers.at(k), getEntryPositions().at(k)));
        }
    }

    return pairs;
}

// Identifiers are uints, so convert them instead of trying to compare strings...
bool SortByIdentifier::operator()(const CrosswordEntry& lhs, const CrosswordEntry& rhs)
{
    bool ok1 = true;
    bool ok2 = true;

    bool comparison = lhs.getIdentifier().toUInt(&ok1) < rhs.getIdentifier().toUInt(&ok2);

    assert(ok1 && ok2);

    return comparison;
}
