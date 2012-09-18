#ifndef XWC3DLOADER_H
#define XWC3DLOADER_H

#include <QStringList>

#include "puzzle3d.h"

namespace XWC3DLoader
{
    bool loadXWC3DMetaData(BCrossword3D& puzzle, QStringList& linelist);
    bool loadXWC3DGrid(BCrossword3D& puzzle, QStringList& linelist);
    bool loadXWC3DClues(BCrossword3D& puzzle, QStringList& linelist);
}

#endif // XWC3DLOADER_H
