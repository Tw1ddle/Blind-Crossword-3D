#ifndef LETTERGRID_H
#define LETTERGRID_H

#include <vector>

#include "letter.h"
#include "uivec3.h"

class LetterGrid
{
public:
    LetterGrid();

    void push_back(Letter letter);

    unsigned int getSize() const;

    Letter* getRefLetterAt(uivec3 location);

    Letter getLetterAt(unsigned int index) const;

    uivec3 getDimensions() const;
    void setDimensions(uivec3 dimensions);

    void clear();
private:
    uivec3 m_GridDimensions;
    std::vector<Letter> m_Letters;
};

#endif // LETTERGRID_H
