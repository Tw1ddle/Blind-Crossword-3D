/*! \brief Classes for loading and saving crosswords.
 *  The loader interface defines the behaviour of classes that implement loading and saving functionality for any crossword format.
 *  The loader class chooses the appropriate loader for a given crossword file format and runs it. It also handles the file I/O for loading and saving crosswords.
 */

#pragma once

#include <QFile>
#include <QObject>
#include <QString>
#include <QStringList>

namespace crossword {

class CrosswordBase;

}

namespace loader {

class CrosswordLoaderInterface {
  public:
    virtual bool loadMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist) = 0;
    virtual bool loadGrid(crossword::CrosswordBase& puzzle, QStringList& linelist) = 0;
    virtual bool loadClues(crossword::CrosswordBase& puzzle, QStringList& linelist) = 0;

    virtual bool saveMetadata(crossword::CrosswordBase& puzzle, QStringList& linelist) = 0;
    virtual bool saveGrid(crossword::CrosswordBase& puzzle, QStringList& linelist) = 0;
    virtual bool saveClues(crossword::CrosswordBase& puzzle, QStringList& linelist) = 0;
};

class CrosswordLoader : public QObject {
    Q_OBJECT
  public:
    CrosswordLoader();

    bool savePuzzle(crossword::CrosswordBase& puzzle, QString filePath, QString extension);
    bool loadPuzzle(crossword::CrosswordBase& puzzle, QString filePath, QString extension);

  private:
    bool readInFile(CrosswordLoaderInterface& loader, crossword::CrosswordBase& puzzle,
                    QStringList& linelist);
    bool writeOutFile(CrosswordLoaderInterface& loader, crossword::CrosswordBase& puzzle, QFile& file);

  signals:
    void loaderError(const QString&, const QString&);
};

}
