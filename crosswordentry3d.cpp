#include "crosswordentry3d.h"

#include <assert.h>

CrosswordEntry3D::CrosswordEntry3D(Direction direction, unsigned int number, uivec3 startingPosition, Word word, Hint hint)
{
    m_Direction = direction;
    m_Number = number;
    m_StartingPosition = startingPosition;
    m_Word = word;
    m_Hint = hint;
}
