#ifndef DIRECTION_H
#define DIRECTION_H

#include <QString>

namespace Directions
{
    typedef QString DIRECTION;

    const DIRECTION ACROSS("Across");
    const DIRECTION AWAY("Away");
    const DIRECTION UP("Up");
    const DIRECTION DOWN("Down");
    const DIRECTION WINDING("Winding");
}

class Direction
{
public:
    Direction();
    Direction(Directions::DIRECTION direction);

    bool operator==(const Direction& direction);
    QString getDirectionName() const;

private:
    Directions::DIRECTION m_Direction;
};

#endif // DIRECTION_H
