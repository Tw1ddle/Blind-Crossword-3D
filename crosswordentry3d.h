#ifndef CROSSWORDENTRY3D_H
#define CROSSWORDENTRY3D_H

#include <QString>

#include "direction.h"
#include "word.h"
#include "uivec3.h"

class CrosswordEntry3D
{
public:
    CrosswordEntry3D(Direction direction, unsigned int identifier, QString number, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, QString clue);

    unsigned int getIdentifier() const;
    QString getEntryName() const;
    QString getSolution() const;
    QString getClue() const;
    QString getSolutionComponentLengths() const;
    Direction getDirection() const;

    uivec3 getStartingPosition() const;
    Word getGuess() const;
    bool isGuessCorrect() const;

    bool intersectsWord(Word* const word) const;

    void resetGuess();
    void setGuess(QString word);

private:
    unsigned int m_Identifier;

    QString m_EntryString;
    Direction m_WordDirection;

    std::vector<unsigned int> m_SolutionComponentLengths;
    QString m_Solution;
    Word m_Guess;
    QString m_Clue;
};

#endif // CROSSWORDENTRY3D_H
