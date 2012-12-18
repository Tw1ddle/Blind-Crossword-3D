#include "crosswordentry.h"

#include <QStringList>

#include <assert.h>

CrosswordEntry::CrosswordEntry(QString direction, QString identifier, QString entry, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, QString clue, std::vector<unsigned int> entryIndices)
{
    m_Identifier = identifier;
    m_WordDirection = direction;
    m_EntryString = entry;
    m_Solution = solution;
    m_Guess = guess;
    m_SolutionComponentLengths = wordComponentLengths;
    m_Clue = clue;
    m_EntryIndices = entryIndices;
}

uivec3 CrosswordEntry::getStartingPosition() const
{
    return m_Guess.getGridLocations().at(0);
}

std::vector<uivec3> CrosswordEntry::getEntryPositions() const
{
    std::vector<uivec3> entryPositions;

    if(m_EntryIndices.empty())
    {
        entryPositions.push_back(m_Guess.getGridLocations().at(0));
        return entryPositions;
    }
    else
    {
        for(unsigned int i = 0; i < m_EntryIndices.size(); i++)
        {
            entryPositions.push_back(getGuess().getGridLocations().at(m_EntryIndices.at(i)));
        }

        return entryPositions;
    }
}

QString CrosswordEntry::getEntryPositionsString() const
{
    QString entryIndices;

    for(unsigned int i = 0; i < m_EntryIndices.size(); i++)
    {
        entryIndices.append(QString::number(m_EntryIndices.at(i) + 1)).append(","); // + 1, we use this for exporting to xwc3d files
    }

    entryIndices.truncate(entryIndices.size() - 1);

    return entryIndices;
}

QString CrosswordEntry::getSolution() const
{
    return m_Solution;
}

QString CrosswordEntry::getClue() const
{
    return m_Clue;
}

Word CrosswordEntry::getGuess() const
{
    return m_Guess;
}

void CrosswordEntry::setGuess(QString word)
{
    assert(m_Guess.getLength() == m_Solution.length());

    m_Guess.setString(word);
}

QString CrosswordEntry::getSolutionComponentLengths() const
{
    QString comp;
    for(unsigned int i = 0; i < m_SolutionComponentLengths.size(); i++)
    {
        comp.append(QString::number(m_SolutionComponentLengths.at(i)));

        if(i != m_SolutionComponentLengths.size() - 1)
        {
            comp.append(",");
        }
    }

    return comp.trimmed();
}

QString CrosswordEntry::getIdentifier() const
{
    return m_Identifier;
}

void CrosswordEntry::resetGuess()
{
    m_Guess.setString(QString(m_Guess.getString().size(), QChar(46)));
}

bool CrosswordEntry::isGuessCorrect() const
{
    if(m_Guess.getString() == m_Solution)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CrosswordEntry::intersectsWord(Word* const word) const
{
    return m_Guess.intersectsWord(word);
}

QString CrosswordEntry::getDirection() const
{
    return m_WordDirection;
}

QString CrosswordEntry::getEntryName() const
{
    return m_EntryString;
}

std::vector<std::pair<unsigned int, uivec3> > CrosswordEntry::getWordEntryStartingPositionPairs() const
{
    std::vector<std::pair<unsigned int, uivec3> > pairs;

    QStringList entries = m_EntryString.split(" ", QString::SkipEmptyParts);
    std::vector<unsigned int> entryNumbers;

    for(int i = 0; i < entries.size(); i++)
    {
        bool converted = false;
        unsigned int entryNumber = entries.at(i).toUInt(&converted);

        if(converted)
        {
            entryNumbers.push_back(entryNumber);
        }
    }

    if(getEntryPositions().size() == entryNumbers.size()) // this should only ever be the case if we have a line like "Stage 2 Anagram" as the entry name
    {
        for(unsigned int k = 0; k < entryNumbers.size(); k++)
        {
            pairs.push_back(std::pair<unsigned int, uivec3>(entryNumbers.at(k), getEntryPositions().at(k)));
        }
    }

    return pairs;
}

bool SortByIdentifier::operator()(const CrosswordEntry& lhs, const CrosswordEntry& rhs)
{
    return lhs.getIdentifier() < rhs.getIdentifier();
}
