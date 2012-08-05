#include "word.h"

Word::Word()
{
}

Word::Word(std::vector<Letter> word)
{
    m_Word = word;
}

unsigned int Word::getLength() const
{
    return m_Word.size();
}

std::vector<uivec3> Word::getLetterPositions() const
{
    std::vector<uivec3> positions;

    for(unsigned int i = 0; i < getLength(); i++)
    {
        positions.push_back(m_Word.at(i).getLocation());
    }

    return positions;
}

QString Word::getString() const
{
    QString word;

    for(unsigned int i = 0; i < m_Word.size(); i++)
    {
        word.push_back(m_Word.at(i).getChar());
    }

    return word;
}
