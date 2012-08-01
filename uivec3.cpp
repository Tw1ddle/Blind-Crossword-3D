#include "uivec3.h"

uivec3::uivec3()
{
    this->x = 0u;
    this->y = 0u;
    this->z = 0u;
}

uivec3::uivec3(unsigned int x, unsigned int y, unsigned int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

unsigned int uivec3::getX()
{
    return x;
}

unsigned int uivec3::getY()
{
    return y;
}

unsigned int uivec3::getZ()
{
    return z;
}

void uivec3::setX(unsigned int x)
{
    this->x = x;
}

void uivec3::setY(unsigned int y)
{
    this->y = y;
}

void uivec3::setZ(unsigned int z)
{
    this->z = z;
}
