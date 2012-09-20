#ifndef XWC3DLOADER_H
#define XWC3DLOADER_H

#include <QStringList>

#include "puzzle3d.h"

class XWC3DLoader
{
public:
    bool loadMetaData(BCrossword3D& puzzle, QStringList& linelist);
    bool loadGrid(BCrossword3D& puzzle, QStringList& linelist);
    bool loadClues(BCrossword3D& puzzle, QStringList& linelist);

private:
    bool loadAcrossClues(BCrossword3D& puzzle, QStringList& linelist);
    bool loadAwayClues(BCrossword3D& puzzle, QStringList& linelist);
    bool loadDownClues(BCrossword3D& puzzle, QStringList& linelist);
    bool loadUpClues(BCrossword3D& puzzle, QStringList& linelist);
    bool loadWindingClues(BCrossword3D& puzzle, QStringList& linelist);
};

#endif // XWC3DLOADER_H
