#ifndef HINT_H
#define HINT_H

#include <QString>

class Hint
{
public:
    Hint();
    Hint(QString hint);

    QString getHint();

private:
    QString m_Hint;
};

#endif // HINT_H
