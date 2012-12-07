/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef LETTERGRID_H
#define LETTERGRID_H

#include <vector>

#include "letter.h"
#include "uivec3.h"

class LetterGrid
{
public:
    LetterGrid();

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

#endif // LETTERGRID_H
