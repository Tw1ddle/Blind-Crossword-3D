#ifndef PUZZLELOADERINTERFACE_H
#define PUZZLELOADERINTERFACE_H

#include <QStringList>
#include <QPixmap>
#include <QDir>

class PuzzleBase;

class PuzzleLoaderInterface
{
public:
    virtual bool loadMetaData(PuzzleBase& puzzle, QStringList& linelist) = 0;
    virtual bool loadGrid(PuzzleBase& puzzle, QStringList& linelist) = 0;
    virtual bool loadClues(PuzzleBase& puzzle, QStringList& linelist) = 0;

protected:
    bool loadBackgroundImage(PuzzleBase& puzzle, QString filename);

private:
    static const QString m_BackgroundImagesFolder;
};

#endif // PUZZLELOADERINTERFACE_H
