#include "discpuzzle.h"

unsigned int DiscPuzzle::toGridIndex(uivec3 index) const
{
    if(index.getY() == 0) // disc center
    {
        return index.getZ() * (getGrid().getDimensions().getY() - 1) * getGrid().getDimensions().getX();
    }
    else
    {
        return 1
                + index.getZ()
                + index.getX()
                + (index.getY() - 1) * getGrid().getDimensions().getX()
                + index.getZ() * (getGrid().getDimensions().getY() - 1) * getGrid().getDimensions().getX();
    }
}
