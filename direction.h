#ifndef DIRECTION_H
#define DIRECTION_H

#include <QString>

namespace Directions
{
    typedef QString DIRECTION;

    const DIRECTION ACROSS("Across");
	const DIRECTION BACKWARDS("Backwards");
	
    const DIRECTION AWAY("Away");
	const DIRECTION TOWARDS("Towards");
	
    const DIRECTION UP("Up");
    const DIRECTION DOWN("Down");

    const DIRECTION CLOCKWISE("Clockwise");
    const DIRECTION ANTICLOCKWISE("Anticlockwise");
    const DIRECTION DIAMETRIC("Diametric");

    const DIRECTION SNAKING("Snaking");
}

class Direction
{
public:
    Direction();
    Direction(Directions::DIRECTION direction);

    bool operator==(const Direction& direction);
    bool operator!=(const Direction& direction);
    QString getString() const;

private:
    Directions::DIRECTION m_Direction;
};

#endif // DIRECTION_H
