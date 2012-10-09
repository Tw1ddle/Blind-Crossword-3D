#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include <QString>
#include <QStringList>
#include <QFile>

#include "puzzleloaderinterface.h"

class PuzzleBase;

class PuzzleLoader : public QObject
{
    Q_OBJECT
public:
    PuzzleLoader();

    bool savePuzzle(PuzzleBase& puzzle, QString filePath, QString extension);
    bool loadPuzzle(PuzzleBase& puzzle, QString filePath, QString extension);

private:
    bool readInFile(PuzzleLoaderInterface& loader, PuzzleBase& puzzle, QStringList& linelist);

    QStringList saveAsXWC(PuzzleBase& puzzle);
    QStringList saveAsXWC3D(PuzzleBase& puzzle);
    QStringList saveAsXWCR3D(PuzzleBase& puzzle);

    bool writeToFile(QStringList& linelist, QFile& file);

signals:
    void loaderError(const QString&, const QString&);
};

#endif // PUZZLELOADER_H
