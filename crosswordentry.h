/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CROSSWORDENTRY_H
#define CROSSWORDENTRY_H

#include <QString>

#include "crosswordword.h"
#include "uivec3.h"

class CrosswordEntry
{
public:
    CrosswordEntry(QString direction, QString identifier, QString number, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, QString clue, std::vector<unsigned int> entryIndices = std::vector<unsigned int>());

    QString getIdentifier() const;

    QString getEntryName() const;
    std::vector<std::pair<unsigned int, uivec3> > getWordEntryStartingPositionPairs() const;
    QString getSolution() const;
    QString getClue() const;
    QString getSolutionComponentLengths() const;
    QString getDirection() const;

    uivec3 getStartingPosition() const;
    std::vector<uivec3> getEntryPositions() const;
    QString getEntryPositionsString() const;

    Word getGuess() const;
    bool isGuessCorrect() const;

    bool intersectsWord(Word* const word) const;

    void resetGuess();
    void setGuess(QString word);

private:
    QString m_Identifier;

    QString m_EntryString;
    QString m_WordDirection;
    QString m_Solution;
    Word m_Guess;
    QString m_Clue;

    std::vector<unsigned int> m_SolutionComponentLengths;
    std::vector<unsigned int> m_EntryIndices;
};

struct SortByIdentifier
{
    bool operator()(const CrosswordEntry& lhs, const CrosswordEntry& rhs);
};

namespace ClueDirections
{
    typedef QString DIRECTION;

    const DIRECTION ACROSS = "Across";
    const DIRECTION BACKWARDS = "Backwards";
    const DIRECTION AWAY = "Away";
    const DIRECTION TOWARDS = "Towards";
    const DIRECTION DOWN = "Down";
    const DIRECTION UP = "Up";
    const DIRECTION CLOCKWISE = "Clockwise";
    const DIRECTION ANTICLOCKWISE = "Anticlockwise";
    const DIRECTION DIAMETRIC = "Diametric";
    const DIRECTION SNAKING = "Snaking";
}

#endif // CROSSWORDENTRY_H
