#ifndef XWCLOADER_H
#define XWCLOADER_H

#include <QStringList>

#include "puzzle3d.h"

class XWCLoader
{
public:
    bool loadMetaData(BCrossword3D& puzzle, QStringList& linelist);
    bool loadGrid(BCrossword3D& puzzle, QStringList& linelist);
    bool loadClues(BCrossword3D& puzzle, QStringList& linelist);

    bool saveMetaData(BCrossword3D& puzzle, QStringList& linelist);
    bool saveGrid(BCrossword3D& puzzle, QStringList& linelist);
    bool saveClues(BCrossword3D& puzzle, QStringList& linelist);
private:
    bool loadAcrossClues(BCrossword3D& puzzle, QStringList& linelist);
    bool loadAwayClues(BCrossword3D& puzzle, QStringList& linelist);
};

#endif // XWCLOADER_H
