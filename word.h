#ifndef WORD_H
#define WORD_H

#include <QString>
#include <vector>

#include "letter.h"
#include "uivec3.h"

class Word
{
public:
    Word();
    Word(QString word);

    QString getWord();
    unsigned int getLength();

private:
    QString m_Word;
    std::vector<Letter*> m_LettersRefs;
};

#endif // WORD_H
