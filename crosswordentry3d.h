#ifndef CROSSWORDENTRY3D_H
#define CROSSWORDENTRY3D_H

#include "word.h"
#include "uivec3.h"
#include "hint.h"
#include "direction.h"

class CrosswordEntry3D
{
public:
    CrosswordEntry3D(Direction direction, unsigned int number, uivec3 startingPosition, Word word, Hint hint);

    uivec3 getStartingPosition();

private:
    Direction m_Direction;
    unsigned int m_Number;
    uivec3 m_StartingPosition;
    Word m_Word;
    Hint m_Hint;
};

#endif // CROSSWORDENTRY3D_H
