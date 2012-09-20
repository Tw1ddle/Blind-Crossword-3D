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

unsigned int uivec3::getX() const
{
    return x;
}

unsigned int uivec3::getY() const
{
    return y;
}

unsigned int uivec3::getZ() const
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

uivec3 uivec3::operator -(const uivec3 &vec) const
{
    return uivec3(this->x - vec.getX(), this->y - vec.getY(), this->z - vec.getZ());
}

bool uivec3::operator ==(const uivec3 &vec) const
{
    return (x == vec.getX() && y == vec.getY() && z == vec.getZ());
}

unsigned int uivec3::toGridIndex(uivec3 gridDimensions) const
{
    return getX() + gridDimensions.getX() * getY() + gridDimensions.getY() * gridDimensions.getX() * getZ();
}
