/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef XWCLOADER_H
#define XWCLOADER_H

#include <QStringList>

#include "crosswordbase.h"
#include "crosswordloader.h"

class XWCLoader : public CrosswordLoaderInterface
{
public:
    bool loadMetaData(CrosswordBase& puzzle, QStringList& linelist);
    bool loadGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool loadClues(CrosswordBase& puzzle, QStringList& linelist);

    bool saveMetaData(CrosswordBase& puzzle, QStringList& linelist);
    bool saveGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool saveClues(CrosswordBase& puzzle, QStringList& linelist);

private:
    bool loadAcrossClues(CrosswordBase& puzzle, QStringList& linelist);
    bool loadAwayClues(CrosswordBase& puzzle, QStringList& linelist);
};

#endif // XWCLOADER_H
