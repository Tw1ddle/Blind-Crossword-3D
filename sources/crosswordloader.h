/*! \brief Classes for loading and saving crosswords.
 *
 *  The loader interface defines the behaviour of classes that implement loading and saving functionality for any crossword format.
 *
 *  The loader class chooses the appropriate loader for a given crossword file format and runs it. It also handles the file I/O for loading and saving crosswords.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

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
