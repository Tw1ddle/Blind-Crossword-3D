#ifndef LETTER_H
#define LETTER_H

#include "uivec3.h"
#include <QChar>

class Letter
{
public:
    Letter(QChar letter, uivec3 location);

    QChar getLetter();
    uivec3 getLocation();

private:
    QChar m_Letter;
    uivec3 m_Location;
};

#endif // LETTER_H
