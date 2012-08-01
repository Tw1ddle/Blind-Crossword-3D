#ifndef DIRECTION_H
#define DIRECTION_H

#include <QString>

namespace Directions
{
    typedef std::pair<unsigned int, QString> DIRECTION;

    const DIRECTION ACROSS(0u, "Across");
    const DIRECTION AWAY(1u, "Away");
    const DIRECTION THROUGH(2u, "Through");
}

class Direction
{
public:
    Direction();
    Direction(Directions::DIRECTION direction);

    void setDirection(Directions::DIRECTION direction);

    QString getDirectionName();
    unsigned int getDirectionType();

private:
    Directions::DIRECTION m_Direction;
};

#endif // DIRECTION_H
