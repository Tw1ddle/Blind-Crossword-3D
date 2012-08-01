#include "direction.h"

Direction::Direction()
{
}

Direction::Direction(Directions::DIRECTION direction)
{
    this->m_Direction = direction;
}

QString Direction::getDirectionName()
{
    return m_Direction.second;
}

unsigned int Direction::getDirectionType()
{
    return m_Direction.first;
}
