/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

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
    bool writeOutFile(PuzzleLoaderInterface& loader, PuzzleBase& puzzle, QFile& file);

signals:
    void loaderError(const QString&, const QString&);
};

#endif // PUZZLELOADER_H
