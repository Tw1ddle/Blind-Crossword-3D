#include "crosswordword.h"

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

bool Word::containsLetter(Letter* letter) const
{
    for(unsigned int i = 0; i < m_Word.size(); i++)
    {
        if(m_Word.at(i) == letter)
        {
            return true;
        }
    }

    return false;
}

bool Word::intersectsWord(Word* word) const
{
    for(unsigned int i = 0; i < getLength(); i++)
    {
        if(word->containsLetter(m_Word.at(i)))
        {
            return true;
        }
    }

    return false;
}

void Word::setString(QString string)
{
    assert(string.length() == m_Word.size());

    for(unsigned int i = 0; i < m_Word.size(); i++)
    {
        m_Word.at(i)->setChar(string.at(i));
    }
}

std::vector<uivec3> Word::getPositions() const
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
