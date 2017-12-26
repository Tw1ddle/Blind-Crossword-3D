/*! \brief Models a crossword grid.
 */

#pragma once

#include <vector>

#include "letter.h"
#include "util/uivec3.h"

class GridData {
  public:
    GridData();

    const Letter* getLetterAt(unsigned int index) const;
    Letter* getRefLetterAt(unsigned int index);

    void push_back(Letter letter);
    std::size_t getSize() const;
    void clear();

    uivec3 getDimensions() const;
    void setDimensions(uivec3 dimensions);

  private:
    uivec3 m_gridDimensions;
    std::vector<Letter> m_letters;
};
