#ifndef DISCPUZZLE_H
#define DISCPUZZLE_H

#include "puzzlebase.h"

class DiscPuzzle : public PuzzleBase
{
public:
    unsigned int toGridIndex(uivec3 index) const;
};

#endif // DISCPUZZLE_H
