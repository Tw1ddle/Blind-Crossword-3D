/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef XWC3DLOADER_H
#define XWC3DLOADER_H

#include <QStringList>

#include "crosswordbase.h"
#include "crosswordloader.h"

class XWC3DLoader : public CrosswordLoaderInterface
{
public:
    bool loadMetaData(CrosswordBase& puzzle, QStringList& linelist);
    bool loadGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool loadClues(CrosswordBase& puzzle, QStringList& linelist);

    bool saveMetaData(CrosswordBase& puzzle, QStringList& linelist);
    bool saveGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool saveClues(CrosswordBase& puzzle, QStringList& linelist);

private:
    bool loadXWC3DGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool loadXWCR3DGrid(CrosswordBase& puzzle, QStringList& linelist);

    bool saveXWC3DGrid(CrosswordBase& puzzle, QStringList& linelist);
    bool saveXWCR3DGrid(CrosswordBase& puzzle, QStringList& linelist);

    bool loadCluesHelper(CrosswordBase& puzzle, QStringList& linelist, QString direction);
    bool loadSnakingClues(CrosswordBase& puzzle, QStringList& linelist, unsigned int numsnaking);

    bool saveCluesHelper(CrosswordBase& puzzle, QStringList& linelist, QString direction);
};

#endif // XWC3DLOADER_H
