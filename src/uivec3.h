/*! \brief Represents a three-component vector of unsigned ints.
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

class uivec3
{
public:
    uivec3(unsigned int x, unsigned int y, unsigned int z);
    uivec3();

    unsigned int getX() const;
    unsigned int getY() const;
    unsigned int getZ() const;
    void setX(unsigned int x);
    void setY(unsigned int y);
    void setZ(unsigned int z);

    uivec3 operator-(const uivec3& vec) const;
    bool operator==(const uivec3& vec) const;

private:
    unsigned int x, y, z;
};
