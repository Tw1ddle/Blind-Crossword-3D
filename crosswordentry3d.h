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
    CrosswordEntry3D(Direction direction, QString number, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, Clue clue);

    QString getEntryString() const;
    Direction getDirection() const;
    QString getSolution() const;
    Clue getClue() const;

    uivec3 getStartingPosition() const;
    Word getGuess() const;
    bool isGuessCorrect() const;

    void resetGuessString();
    void setGuessString(QString word);

    QString getWordComponentsString() const;

private:
    QString m_EntryString;
    Direction m_WordDirection;

    std::vector<unsigned int> m_WordComponentLengths;

    QString m_SolutionString;
    Word m_GuessedWord;
    Clue m_Clue;
};

#endif // CROSSWORDENTRY3D_H
