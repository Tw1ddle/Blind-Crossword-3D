#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include <QString>
#include <QStringList>
#include <QFile>

class PuzzleBase;

class PuzzleLoaderInterface
{
public:
    virtual bool loadMetaData(PuzzleBase& puzzle, QStringList& linelist) = 0;
    virtual bool loadGrid(PuzzleBase& puzzle, QStringList& linelist) = 0;
    virtual bool loadClues(PuzzleBase& puzzle, QStringList& linelist) = 0;

    virtual bool saveMetaData(PuzzleBase& puzzle, QStringList& linelist) = 0;
    virtual bool saveGrid(PuzzleBase& puzzle, QStringList& linelist) = 0;
    virtual bool saveClues(PuzzleBase& puzzle, QStringList& linelist) = 0;
};

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
