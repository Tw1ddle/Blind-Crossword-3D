#include "direction.h"

Direction::Direction()
{
}

Direction::Direction(Directions::DIRECTION direction)
{
    this->m_Direction = direction;
}

QString Direction::getDirectionName() const
{
    return m_Direction.second;
}

unsigned int Direction::getDirectionType() const
{
    return m_Direction.first;
}
