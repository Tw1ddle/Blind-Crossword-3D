/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CROSSWORDENTRY3D_H
#define CROSSWORDENTRY3D_H

#include <QString>

#include "direction.h"
#include "word.h"
#include "uivec3.h"

class CrosswordEntry3D
{
public:
    CrosswordEntry3D(Direction direction, unsigned int identifier, QString number, QString solution, Word guess, std::vector<unsigned int> wordComponentLengths, QString clue, std::vector<unsigned int> entryIndices = std::vector<unsigned int>());

    unsigned int getIdentifier() const;
    QString getEntryName() const;
    std::vector<std::pair<unsigned int, uivec3> > getWordEntryStartingPositionPairs() const;
    QString getSolution() const;
    QString getClue() const;
    QString getSolutionComponentLengths() const;
    Direction getDirection() const;

    uivec3 getStartingPosition() const;
    std::vector<uivec3> getEntryPositions() const;
    QString getEntryPositionsString() const;

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
    std::vector<unsigned int> m_EntryIndices;
    QString m_Solution;
    Word m_Guess;
    QString m_Clue;
};

struct SortByIdentifier
{
    bool operator()(const CrosswordEntry3D& lhs, const CrosswordEntry3D& rhs);
};

#endif // CROSSWORDENTRY3D_H
