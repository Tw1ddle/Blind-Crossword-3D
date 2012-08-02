#ifndef WORD_H
#define WORD_H

#include <QString>
#include <vector>

#include "letter.h"

class Word
{
public:
    Word();
    Word(std::vector<Letter> word);

    QString getString();
    unsigned int getLength();

private:
    std::vector<Letter> m_Word;
};

#endif // WORD_H
