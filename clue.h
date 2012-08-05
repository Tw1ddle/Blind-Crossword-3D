#ifndef CLUE_H
#define CLUE_H

#include <QString>

class Clue
{
public:
    Clue();
    Clue(QString clue);

    QString getString() const;

private:
    QString m_Clue;
};

#endif // CLUE_H
