/*! \brief Models a crossword grid.
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CROSSWORDGRID_H
#define CROSSWORDGRID_H

#include <vector>

#include "letter.h"
#include "uivec3.h"

class GridData
{
public:
    GridData();

    const Letter* getLetterAt(unsigned int index) const;
    Letter* getRefLetterAt(unsigned int index);

    void push_back(Letter letter);
    unsigned int getSize() const;
    void clear();

    uivec3 getDimensions() const;
    void setDimensions(uivec3 dimensions);

private:
    uivec3 m_GridDimensions;
    std::vector<Letter> m_Letters;
};

#endif // CROSSWORDGRID_H
