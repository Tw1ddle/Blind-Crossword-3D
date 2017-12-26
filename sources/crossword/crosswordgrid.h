/*! \brief Models a crossword grid.
 */

#pragma once

#include <vector>

#include "crossword/letter.h"
#include "util/uivec3.h"

namespace crossword {

class GridData {
  public:
    GridData();

    const Letter* getLetterAt(unsigned int index) const;
    Letter* getRefLetterAt(unsigned int index);

    void push_back(Letter letter);
    std::size_t getSize() const;
    void clear();

    util::uivec3 getDimensions() const;
    void setDimensions(util::uivec3 dimensions);

  private:
    util::uivec3 m_gridDimensions;
    std::vector<Letter> m_letters;
};

}
