#include "crosswordentry3d.h"

#include <assert.h>

CrosswordEntry3D::CrosswordEntry3D(Direction direction, QString entry, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, Clue clue)
{
    m_WordDirection = direction;
    m_EntryString = entry;
    m_Solution = solution;
    m_Guess = guess;
    m_SolutionComponentLengths = wordComponentLengths;
    m_Clue = clue;
}

uivec3 CrosswordEntry3D::getStartingPosition() const
{
    return m_Guess.getGridLocations().at(0);
}

QString CrosswordEntry3D::getSolution() const
{
    return m_Solution;
}

Clue CrosswordEntry3D::getClue() const
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

bool CrosswordEntry3D::containsLetter(Letter* letter) const
{
    return m_Guess.containsLetter(letter);
}

Direction CrosswordEntry3D::getDirection() const
{
    return m_WordDirection;
}

QString CrosswordEntry3D::getEntryName() const
{
    return m_EntryString;
}
