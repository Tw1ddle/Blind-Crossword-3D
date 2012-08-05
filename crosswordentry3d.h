#ifndef CROSSWORDENTRY3D_H
#define CROSSWORDENTRY3D_H

#include "word.h"
#include "uivec3.h"
#include "clue.h"
#include "direction.h"

class CrosswordEntry3D
{
public:
    CrosswordEntry3D(Direction direction, unsigned int number, Word word, Clue clue);

    uivec3 getStartingPosition() const;
    Word getSolution() const;
    Clue getClue() const;

    QString getGuess() const;
    void setGuess(QString word);

private:
    unsigned int m_EntryNumber;
    Direction m_WordDirection;
    Word m_SolutionWord;
    QString m_GuessedWord;
    Clue m_Clue;
};

#endif // CROSSWORDENTRY3D_H
