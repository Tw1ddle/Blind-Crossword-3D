#include "direction.h"

Direction::Direction()
{
}

Direction::Direction(Directions::DIRECTION direction)
{
    this->m_Direction = direction;
}

bool Direction::operator ==(const Direction &direction)
{
    return this->getDirectionName() == direction.getDirectionName();
}

bool Direction::operator !=(const Direction &direction)
{
    return !(this->getDirectionName() == direction.getDirectionName());
}

QString Direction::getDirectionName() const
{
    return m_Direction;
}
