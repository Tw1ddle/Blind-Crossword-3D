/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CROSSWORDLOADER_H
#define CROSSWORDLOADER_H

#include <QString>
#include <QStringList>
#include <QFile>

class CrosswordBase;

class CrosswordLoaderInterface
{
public:
    virtual bool loadMetaData(CrosswordBase& puzzle, QStringList& linelist) = 0;
    virtual bool loadGrid(CrosswordBase& puzzle, QStringList& linelist) = 0;
    virtual bool loadClues(CrosswordBase& puzzle, QStringList& linelist) = 0;

    virtual bool saveMetaData(CrosswordBase& puzzle, QStringList& linelist) = 0;
    virtual bool saveGrid(CrosswordBase& puzzle, QStringList& linelist) = 0;
    virtual bool saveClues(CrosswordBase& puzzle, QStringList& linelist) = 0;
};

class CrosswordLoader : public QObject
{
    Q_OBJECT
public:
    CrosswordLoader();

    bool savePuzzle(CrosswordBase& puzzle, QString filePath, QString extension);
    bool loadPuzzle(CrosswordBase& puzzle, QString filePath, QString extension);

private:
    bool readInFile(CrosswordLoaderInterface& loader, CrosswordBase& puzzle, QStringList& linelist);
    bool writeOutFile(CrosswordLoaderInterface& loader, CrosswordBase& puzzle, QFile& file);

signals:
    void loaderError(const QString&, const QString&);
};

#endif // CROSSWORDLOADER_H
