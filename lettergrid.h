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

    Letter getLetterAtLocation(uivec3 location) const;
    void setLetterAtLocation(QChar letter, uivec3 location);

    Letter getLetterAtIndex(unsigned int index) const;
    void setLetterAtIndex(QChar letter, unsigned int index);

    uivec3 getDimensions() const;
    void setDimensions(uivec3 dimensions);

    void clear();

    static uivec3 indexToGrid(unsigned int index, uivec3 gridDimensions);
    static unsigned int gridToIndex(uivec3 gridCoordinate, uivec3 gridDimensions);

private:
    unsigned int getIndex(uivec3 location) const;
    uivec3 m_GridDimensions;
    std::vector<Letter> m_Letters;
};

#endif // LETTERGRID_H
