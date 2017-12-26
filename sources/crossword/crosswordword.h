/*! \brief Models a word in the crossword grid.
 *  A word consists of letters, which reference particular coordinates within a grid.
 */

#pragma once

#include <vector>

#include <QString>

#include "crossword/letter.h"
#include "util/uivec3.h"

namespace crossword {

class Word {
  public:
    Word();
    Word(std::vector<Letter*> word);

    std::vector<util::uivec3> getPositions() const;
    QString getString() const;
    std::size_t getLength() const;
    bool containsLetter(Letter* letter) const; // compares pointers
    bool intersectsWord(Word* word) const; // compares pointers

    void setString(QString string);

  private:
    std::vector<Letter*> m_word;
};

}
