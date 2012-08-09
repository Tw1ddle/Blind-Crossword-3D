#include "crosswordentry3d.h"

#include <assert.h>

CrosswordEntry3D::CrosswordEntry3D(Direction direction, unsigned int number, QString solution, Word guess, Clue clue)
{
    m_WordDirection = direction;
    m_EntryNumber = number;
    m_SolutionString = solution;
    m_GuessedWord = guess;
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

unsigned int CrosswordEntry3D::getEntryNumber() const
{
    return m_EntryNumber;
}
