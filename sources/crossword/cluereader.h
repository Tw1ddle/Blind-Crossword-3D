/*! \brief Breaks clues down into individual words.
 *  Enables a user to advance through a body of text word by word.
 */

#pragma once

#include <QObject>
#include <QString>

#include "crossword/crosswordentry.h"

namespace crossword {

class ClueReader : public QObject {
    Q_OBJECT

  public:
    ClueReader();

    QString getWord() const;
    QString advanceWord();

  public slots:
    void setText(CrosswordEntry entry);

  private:
    QString m_text;
    unsigned int m_index;
    const QChar m_separator;
};

}
