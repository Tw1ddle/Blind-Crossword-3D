#include "crossword/crosswordword.h"

#include <cassert>

Word::Word()
{
}

Word::Word(std::vector<Letter*> word)
{
    m_word = word;
}

std::size_t Word::getLength() const
{
    return m_word.size();
}

bool Word::containsLetter(Letter* letter) const
{
    for (unsigned int i = 0; i < m_word.size(); i++) {
        if (m_word.at(i) == letter) {
            return true;
        }
    }

    return false;
}

bool Word::intersectsWord(Word* word) const
{
    for (unsigned int i = 0; i < getLength(); i++) {
        if (word->containsLetter(m_word.at(i))) {
            return true;
        }
    }

    return false;
}

void Word::setString(QString string)
{
    assert(string.length() == m_word.size());

    for (unsigned int i = 0; i < m_word.size(); i++) {
        m_word.at(i)->setChar(string.at(i));
    }
}

std::vector<util::uivec3> Word::getPositions() const
{
    std::vector<util::uivec3> positions;

    for (unsigned int i = 0; i < getLength(); i++) {
        positions.push_back(m_word.at(i)->getLocation());
    }

    return positions;
}

QString Word::getString() const
{
    QString word;

    for (unsigned int i = 0; i < m_word.size(); i++) {
        word.push_back(m_word.at(i)->getChar());
    }

    return word;
}
