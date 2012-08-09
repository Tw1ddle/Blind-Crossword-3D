#ifndef CROSSWORDENTRY3D_H
#define CROSSWORDENTRY3D_H

#include <QString>

#include "direction.h"
#include "word.h"
#include "uivec3.h"
#include "clue.h"

class CrosswordEntry3D
{
public:
    CrosswordEntry3D(Direction direction, unsigned int number, QString solution, Word guess, Clue clue);

    unsigned int getEntryNumber() const;
    Direction getDirection() const;
    QString getSolution() const;
    Clue getClue() const;

    uivec3 getStartingPosition() const;
    Word getGuess() const;
    bool isGuessCorrect() const;

    void resetGuessString();
    void setGuessString(QString word);

private:
    unsigned int m_EntryNumber;
    Direction m_WordDirection;
    QString m_SolutionString;
    Word m_GuessedWord;
    Clue m_Clue;
};

#endif // CROSSWORDENTRY3D_H
