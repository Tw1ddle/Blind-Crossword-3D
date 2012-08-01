#ifndef UIVEC3_H
#define UIVEC3_H

class uivec3
{
public:
    uivec3(unsigned int x, unsigned int y, unsigned int z);
    uivec3();

    unsigned int getX();
    unsigned int getY();
    unsigned int getZ();
    void setX(unsigned int x);
    void setY(unsigned int y);
    void setZ(unsigned int z);

private:
    unsigned int x, y, z;
};

#endif // UIVEC3_H
