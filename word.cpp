#include "word.h"

#include <assert.h>

Word::Word()
{
}

Word::Word(std::vector<Letter*> word)
{
    m_Word = word;
}

unsigned int Word::getLength() const
{
    return m_Word.size();
}

void Word::setString(QString string)
{
    assert(string.length() == m_Word.size());

    for(unsigned int i = 0; i < m_Word.size(); i++)
    {
        m_Word.at(i)->setChar(string.at(i));
    }
}

std::vector<uivec3> Word::getGridLocations() const
{
    std::vector<uivec3> positions;

    for(unsigned int i = 0; i < getLength(); i++)
    {
        positions.push_back(m_Word.at(i)->getLocation());
    }

    return positions;
}

QString Word::getString() const
{
    QString word;

    for(unsigned int i = 0; i < m_Word.size(); i++)
    {
        word.push_back(m_Word.at(i)->getChar());
    }

    return word;
}
