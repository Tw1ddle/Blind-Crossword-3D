#include "cluedirection.h"

ClueDirection::ClueDirection()
{
}

ClueDirection::ClueDirection(ClueDirections::DIRECTION direction)
{
    this->m_Direction = direction;
}

bool ClueDirection::operator ==(const ClueDirection &direction)
{
    return this->getString() == direction.getString();
}

bool ClueDirection::operator !=(const ClueDirection &direction)
{
    return !(this->getString() == direction.getString());
}

QString ClueDirection::getString() const
{
    return m_Direction;
}
