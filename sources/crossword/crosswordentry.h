/*! \brief A crossword entry.
 *  Models a clue in a crossword and the information associated with a clue.
 */

#pragma once

#include <QString>

#include "crossword/crosswordword.h"
#include "util/uivec3.h"

class CrosswordEntry
{
public:
    CrosswordEntry(QString direction, QString identifier, QString number, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, QString clue, std::vector<unsigned int> entryIndices = std::vector<unsigned int>());

    QString getDirection() const;
    QString getIdentifier() const;
    QString getEntry() const;
    Word getGuess() const;
    QString getSolution() const;
    QString getClue() const;
    QString getSolutionComponentLengths() const;

    uivec3 getStartingPosition() const;
    std::vector<uivec3> getEntryPositions() const;
    QString getEntryPositionsString() const;

    std::vector<std::pair<unsigned int, uivec3> > getWordEntryStartingPositionPairs() const;

    bool isGuessCorrect() const;

    bool intersectsWord(Word* word) const;

    void resetGuess();
    void setGuess(QString word);

private:
    QString m_id;

    QString m_entryString;
    QString m_wordDirection;
    QString m_solution;
    Word m_guess;
    QString m_clue;

    std::vector<unsigned int> m_solutionComponentLengths;
    std::vector<unsigned int> m_entryIndices;
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
