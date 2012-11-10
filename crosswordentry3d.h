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
    CrosswordEntry3D(Direction direction, unsigned int identifier, QString number, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, Clue clue);

    QString getEntryName() const;
    Direction getDirection() const;
    QString getSolution() const;
    Clue getClue() const;
    QString getSolutionComponentLengths() const;
    unsigned int getIdentifier() const;

    uivec3 getStartingPosition() const;
    Word getGuess() const;
    bool isGuessCorrect() const;

    bool containsLetter(Letter* letter) const; // compares pointers/addresses of the letters, not the character values

    void resetGuess();
    void setGuess(QString word);

private:
    unsigned int m_Identifier;

    QString m_EntryString;
    Direction m_WordDirection;

    std::vector<unsigned int> m_SolutionComponentLengths;
    QString m_Solution;
    Word m_Guess;
    Clue m_Clue;
};

#endif // CROSSWORDENTRY3D_H
