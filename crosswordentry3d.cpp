#include "crosswordentry3d.h"

#include <assert.h>

CrosswordEntry3D::CrosswordEntry3D(Direction direction, QString entry, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, Clue clue)
{
    m_WordDirection = direction;
    m_EntryString = entry;
    m_SolutionString = solution;
    m_GuessedWord = guess;
    m_WordComponentLengths = wordComponentLengths;
    m_Clue = clue;
}

uivec3 CrosswordEntry3D::getStartingPosition() const
{
    return m_GuessedWord.getGridLocations().at(0);
}

QString CrosswordEntry3D::getSolution() const
{
    return m_SolutionString;
}

Clue CrosswordEntry3D::getClue() const
{
    return m_Clue;
}

Word CrosswordEntry3D::getGuess() const
{
    return m_GuessedWord;
}

void CrosswordEntry3D::setGuessString(QString word)
{
    assert(m_GuessedWord.getLength() == m_SolutionString.length());

    m_GuessedWord.setString(word);
}

QString CrosswordEntry3D::getWordComponentsString() const
{
    QString comp;
    for(unsigned int i = 0; i < m_WordComponentLengths.size(); i++)
    {
        comp.append(QString::number(m_WordComponentLengths.at(i)));

        if(i != m_WordComponentLengths.size() - 1)
        {
            comp.append(",");
        }
    }

    return comp.trimmed();
}

void CrosswordEntry3D::resetGuessString()
{
    m_GuessedWord.setString(QString(m_GuessedWord.getString().size(), QChar(46)));
}

bool CrosswordEntry3D::isGuessCorrect() const
{
    if(m_GuessedWord.getString() == m_SolutionString)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Direction CrosswordEntry3D::getDirection() const
{
    return m_WordDirection;
}

QString CrosswordEntry3D::getEntryString() const
{
    return m_EntryString;
}
