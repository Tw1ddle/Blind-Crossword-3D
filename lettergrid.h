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

    Letter getLetterAt(uivec3 location) const;
    Letter getLetterAt(unsigned int index) const;
    void setLetterAt(QChar letter, uivec3 location);
    void setLetterAt(QChar letter, unsigned int index);

    uivec3 getDimensions() const;
    void setDimensions(uivec3 dimensions);

    void clear();
private:
    unsigned int getIndex(uivec3 location) const;
    unsigned int getIndex2(uivec3 location) const;
    uivec3 m_GridDimensions;
    std::vector<Letter> m_Letters;
};

#endif // LETTERGRID_H
