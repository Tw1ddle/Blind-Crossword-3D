/*! \brief Models a word in the crossword grid.
 *
 *
 *  A word consists of letters, which reference particular coordinates within a grid.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CROSSWORDWORD_H
#define CROSSWORDWORD_H

#include <QString>
#include <vector>

#include "letter.h"
#include "uivec3.h"

class Word
{
public:
    Word();
    Word(std::vector<Letter*> word);

    std::vector<uivec3> getGridLocations() const;
    QString getString() const;
    unsigned int getLength() const;
    bool containsLetter(Letter* letter) const; // compares pointers
    bool intersectsWord(Word* word) const; // compares pointers

    void setString(QString string);

private:
    std::vector<Letter*> m_Word;
};

#endif // CROSSWORDWORD_H
