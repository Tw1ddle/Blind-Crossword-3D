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
    return this->getString() == direction.getString();
}

bool Direction::operator !=(const Direction &direction)
{
    return !(this->getString() == direction.getString());
}

QString Direction::getString() const
{
    return m_Direction;
}
