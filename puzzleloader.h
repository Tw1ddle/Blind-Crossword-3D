#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include <QString>
#include <QStringList>
#include <QFile>

#include "direction.h"

class BCrossword3D;

namespace FileFormats
{
    typedef QString FORMAT;

    const FORMAT XWC3D = "xwc3d";
    const FORMAT XWC = "xwc";
}

class PuzzleLoader : public QObject
{
    Q_OBJECT
public:
    PuzzleLoader();

    bool savePuzzle(BCrossword3D& puzzle, QString filePath, QString extension);
    bool loadPuzzle(BCrossword3D& puzzle, QString filePath, QString extension);

private:
    bool readInXWC3D(BCrossword3D& puzzle, QStringList& linelist);
    bool readInXWC(BCrossword3D& puzzle, QStringList& linelist);

    QStringList saveAsXWC(BCrossword3D& puzzle);
    QStringList saveAsXWC3D(BCrossword3D& puzzle);

    QStringList saveXWC3DCrosswordEntryBlock(QStringList& linelist, Direction entryDirection);

    bool writeToFile(QStringList& linelist, QFile& file);

signals:
    void loaderError(const QString&, const QString&);
};

#endif // PUZZLELOADER_H
