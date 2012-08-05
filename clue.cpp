#include "clue.h"

Clue::Clue()
{
}

Clue::Clue(QString clue)
{
    m_Clue = clue;
}

QString Clue::getString() const
{
    return m_Clue;
}
