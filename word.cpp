#include "word.h"

Word::Word()
{
    m_Word = "";
}

Word::Word(QString word)
{
    m_Word = word;
}

unsigned int Word::getLength()
{
    return m_Word.length();
}

QString Word::getWord()
{
    return m_Word;
}
