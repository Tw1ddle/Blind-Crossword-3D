/*! \brief Models a letter in a crossword puzzle.
 *  A letter is modelled as a location in the crossword grid and a letter.
 */

#pragma once

#include <QChar>

#include "util/uivec3.h"

namespace crossword {

class Letter {
  public:
    Letter(QChar letter, util::uivec3 location);

    void setChar(QChar letter);
    QChar getChar() const;
    util::uivec3 getLocation() const;
    bool operator==(const Letter& letter) const;

  private:
    QChar m_letter;
    util::uivec3 m_location;
};

}
