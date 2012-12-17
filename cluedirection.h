/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */
#ifndef CLUEDIRECTION_H
#define CLUEDIRECTION_H

#include <QString>

namespace ClueDirections
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

class ClueDirection
{
public:
    ClueDirection();
    ClueDirection(ClueDirections::DIRECTION direction);

    bool operator==(const ClueDirection& direction);
    bool operator!=(const ClueDirection& direction);
    QString getString() const;

private:
    ClueDirections::DIRECTION m_Direction;
};

#endif // CLUEDIRECTION_H
