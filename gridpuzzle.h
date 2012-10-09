#ifndef GRIDPUZZLE_H
#define GRIDPUZZLE_H

#include "puzzlebase.h"

class GridPuzzle : public PuzzleBase
{
public:
    unsigned toGridIndex(uivec3 index) const;
};

#endif // GRIDPUZZLE_H
