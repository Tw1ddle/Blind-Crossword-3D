/*! \brief Models a letter in a crossword puzzle.
 *
 *  A letter is modelled as a location in the crossword grid and a letter.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef LETTER_H
#define LETTER_H

#include <QChar>

#include "uivec3.h"

class Letter
{
public:
    Letter(QChar letter, uivec3 location);

    void setChar(QChar letter);
    QChar getChar() const;
    uivec3 getLocation() const;
    bool operator==(const Letter& letter) const;

private:
    QChar m_Letter;
    uivec3 m_Location;
};

#endif // LETTER_H
