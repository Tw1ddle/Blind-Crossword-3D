#include "crosswordentry3d.h"

#include <assert.h>

CrosswordEntry3D::CrosswordEntry3D(Direction direction, unsigned int number, uivec3 startingPosition, Word word, Hint hint)
{
    m_WordDirection = direction;
    m_EntryNumber = number;
    m_StartingPosition = startingPosition;
    m_SolutionWord = word;
    m_Hint = hint;

    m_GuessedWord = QString(m_SolutionWord.getLength(), QChar(46));
}

Word CrosswordEntry3D::getSolution()
{
    return m_SolutionWord;
}

Hint CrosswordEntry3D::getHint()
{
    return m_Hint;
}

QString CrosswordEntry3D::getGuess()
{
    return m_GuessedWord;
}

void CrosswordEntry3D::setGuess(QString word)
{
    assert(m_GuessedWord.length() == m_SolutionWord.getLength());

    m_GuessedWord = word;
}
