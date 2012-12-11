#include "crosswordentry3d.h"

#include <QStringList>

#include <assert.h>

CrosswordEntry3D::CrosswordEntry3D(Direction direction, unsigned int identifier, QString entry, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, QString clue, std::vector<unsigned int> entryIndices)
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

uivec3 CrosswordEntry3D::getStartingPosition() const
{
    return m_Guess.getGridLocations().at(0);
}

std::vector<uivec3> CrosswordEntry3D::getEntryPositions() const
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

QString CrosswordEntry3D::getEntryPositionsString() const
{
    QString entryIndices;

    for(unsigned int i = 0; i < m_EntryIndices.size(); i++)
    {
        entryIndices.append(QString::number(m_EntryIndices.at(i) + 1)).append(","); // + 1, we use this for exporting to xwc3d files
    }

    entryIndices.truncate(entryIndices.size() - 1);

    return entryIndices;
}

QString CrosswordEntry3D::getSolution() const
{
    return m_Solution;
}

QString CrosswordEntry3D::getClue() const
{
    return m_Clue;
}

Word CrosswordEntry3D::getGuess() const
{
    return m_Guess;
}

void CrosswordEntry3D::setGuess(QString word)
{
    assert(m_Guess.getLength() == m_Solution.length());

    m_Guess.setString(word);
}

QString CrosswordEntry3D::getSolutionComponentLengths() const
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

unsigned int CrosswordEntry3D::getIdentifier() const
{
    return m_Identifier;
}

void CrosswordEntry3D::resetGuess()
{
    m_Guess.setString(QString(m_Guess.getString().size(), QChar(46)));
}

bool CrosswordEntry3D::isGuessCorrect() const
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

bool CrosswordEntry3D::intersectsWord(Word* const word) const
{
    return m_Guess.intersectsWord(word);
}

Direction CrosswordEntry3D::getDirection() const
{
    return m_WordDirection;
}

QString CrosswordEntry3D::getEntryName() const
{
    return m_EntryString;
}

std::vector<std::pair<unsigned int, uivec3> > CrosswordEntry3D::getWordEntryStartingPositionPairs() const
{
    std::vector<std::pair<unsigned int, uivec3> > pairs;

    QStringList entries = m_EntryString.split(" ", QString::SkipEmptyParts);
    std::vector<unsigned int> entryNumbers;

    for(unsigned int i = 0; i < entries.size(); i++)
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
