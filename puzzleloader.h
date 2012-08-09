#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include <QString>
#include <QStringList>

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

    void savePuzzle(BCrossword3D& puzzle, QString filePath, QString extension);
    void loadPuzzle(BCrossword3D& puzzle, QString filePath, QString extension);

private:
    void readInXWC3D(BCrossword3D &puzzle, QStringList& linelist);
    void readInXWC(BCrossword3D &puzzle, QStringList& linelist);

    QStringList saveAsXWC(BCrossword3D &puzzle);
    QStringList saveXWC3DCrosswordEntryBlock(QStringList& linelist, Direction entryDirection);

    void writeToFile(QStringList& linelist);

signals:
    void loaderError(const QString&, const QString&);
};

#endif // PUZZLELOADER_H
