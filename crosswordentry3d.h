#ifndef CROSSWORDENTRY3D_H
#define CROSSWORDENTRY3D_H

#include "word.h"
#include "uivec3.h"
#include "clue.h"
#include "direction.h"

class CrosswordEntry3D
{
public:
    CrosswordEntry3D(Direction direction, unsigned int number, QString solution, Word guess, Clue clue);

    uivec3 getStartingPosition() const;
    Letter getStartingLetter() const;
    QString getSolution() const;
    Clue getClue() const;
    Direction getDirection() const;
    unsigned int getEntryNumber() const;

    bool isGuessCorrect() const;
    Word getGuess() const;
    void setGuessString(QString word);

    void resetGuessString();

private:
    unsigned int m_EntryNumber;
    Direction m_WordDirection;
    QString m_SolutionString;
    Word m_GuessedWord;
    Clue m_Clue;
};

#endif // CROSSWORDENTRY3D_H
