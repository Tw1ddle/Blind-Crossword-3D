#include "crosswordentry3d.h"

#include <assert.h>

CrosswordEntry3D::CrosswordEntry3D(Direction direction, unsigned int number, Word word, Clue clue)
{
    m_WordDirection = direction;
    m_EntryNumber = number;
    m_SolutionWord = word;
    m_Clue = clue;

    m_GuessedWord = QString(m_SolutionWord.getLength(), QChar(46));
}

uivec3 CrosswordEntry3D::getStartingPosition() const
{
    return m_SolutionWord.getLetterPositions().at(0);
}

Word CrosswordEntry3D::getSolution() const
{
    return m_SolutionWord;
}

Clue CrosswordEntry3D::getClue() const
{
    return m_Clue;
}

QString CrosswordEntry3D::getGuess() const
{
    return m_GuessedWord;
}

void CrosswordEntry3D::setGuess(QString word)
{
    assert(m_GuessedWord.length() == m_SolutionWord.getLength());

    m_GuessedWord = word;
}
