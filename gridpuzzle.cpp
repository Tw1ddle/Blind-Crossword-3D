#include "gridpuzzle.h"

unsigned GridPuzzle::toGridIndex(uivec3 index) const
{
    return index.getX() + getGrid().getDimensions().getX() * index.getY() + getGrid().getDimensions().getY() * getGrid().getDimensions().getX() * index.getZ();
}
