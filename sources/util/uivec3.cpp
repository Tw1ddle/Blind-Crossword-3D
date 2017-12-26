#include "util/uivec3.h"

namespace util {

uivec3::uivec3()
{
    this->m_x = 0u;
    this->m_y = 0u;
    this->m_z = 0u;
}

uivec3::uivec3(unsigned int x, unsigned int y, unsigned int z)
{
    this->m_x = x;
    this->m_y = y;
    this->m_z = z;
}

unsigned int uivec3::getX() const
{
    return m_x;
}

unsigned int uivec3::getY() const
{
    return m_y;
}

unsigned int uivec3::getZ() const
{
    return m_z;
}

void uivec3::setX(unsigned int x)
{
    this->m_x = x;
}

void uivec3::setY(unsigned int y)
{
    this->m_y = y;
}

void uivec3::setZ(unsigned int z)
{
    this->m_z = z;
}

uivec3 uivec3::operator -(const uivec3& vec) const
{
    return uivec3(this->m_x - vec.m_x, this->m_y - vec.m_y, this->m_z - vec.m_z);
}

bool uivec3::operator ==(const uivec3& vec) const
{
    return (m_x == vec.getX() && m_y == vec.m_y && m_z == vec.m_z);
}

}
