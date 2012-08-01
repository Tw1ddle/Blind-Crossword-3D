#include "hint.h"

Hint::Hint()
{
}

Hint::Hint(QString hint)
{
    m_Hint = hint;
}

QString Hint::getHint()
{
    return m_Hint;
}
