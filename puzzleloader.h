#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include <QString>
#include <QStringList>

class Puzzle3D;

namespace FileFormats
{
    typedef QString FORMAT;

    const FORMAT XWC3D = ".xwc3d";
    const FORMAT XWC = ".xwc";
}

class PuzzleLoader
{
public:
    PuzzleLoader();

    void loadPuzzle(Puzzle3D& puzzle, QString filePath);
    void savePuzzle(Puzzle3D& puzzle, QString filePath);

private:
    void readInXWC3D(Puzzle3D &puzzle, QStringList& linelist);

    void readInXWC(Puzzle3D &puzzle, QStringList& linelist);

    FileFormats::FORMAT determineFormat(QStringList& linelist);

    bool m_LettersLoaded;
};

#endif // PUZZLELOADER_H
