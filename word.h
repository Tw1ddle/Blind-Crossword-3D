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
    Word(std::vector<Letter*> word);

    std::vector<uivec3> getGridLocations() const;
    QString getString() const;
    unsigned int getLength() const;

    void setString(QString string);

private:
    std::vector<Letter*> m_Word;
};

#endif // WORD_H
