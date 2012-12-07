/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef ADVANCEDCLUEREADER_H
#define ADVANCEDCLUEREADER_H

#include <QString>
#include <QStringList>

#include "crosswordentry3d.h"

class ClueReader : public QObject
{
    Q_OBJECT

public:
    ClueReader();

    QString getWord();
    QString advanceWord();

public slots:
    void setText(CrosswordEntry3D entry);

private:
    QString text;
    unsigned int index;
    const QChar separator;
};

#endif // ADVANCEDCLUEREADER_H
