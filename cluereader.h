/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
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
