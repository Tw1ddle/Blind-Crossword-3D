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

    QString getWord() const;
    QString advanceWord();

public slots:
    void setText(CrosswordEntry entry);

private:
    QString m_Text;
    unsigned int m_Index;
    const QChar m_Separator;
};

#endif // CLUEREADER_H
