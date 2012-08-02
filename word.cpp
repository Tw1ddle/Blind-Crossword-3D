#include "word.h"

Word::Word()
{
}

Word::Word(std::vector<Letter> word)
{
    m_Word = word;
}

unsigned int Word::getLength()
{
    return m_Word.size();
}

QString Word::getString()
{
    QString word;

    for(unsigned int i = 0; i < m_Word.size(); i++)
    {
        word.push_back(m_Word.at(i).getLetter());
    }

    return word;
}
