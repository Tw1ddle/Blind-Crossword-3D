/*! \brief Breaks clues down into individual words.
 *
 *
 *  Enables a user to advance through a body of text word by word.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CLUEREADER_H
#define CLUEREADER_H

#include <QString>
#include <QStringList>

#include "crosswordentry.h"

class ClueReader : public QObject
{
    Q_OBJECT

public:
    ClueReader();

    QString getWord();
    QString advanceWord();

public slots:
    void setText(CrosswordEntry entry);

private:
    QString text;
    unsigned int index;
    const QChar separator;
};

#endif // CLUEREADER_H
