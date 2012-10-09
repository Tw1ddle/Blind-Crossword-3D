#include "puzzleloaderinterface.h"

#include "puzzlebase.h"

const QString PuzzleLoaderInterface::m_BackgroundImagesFolder = "/Backgrounds";

bool PuzzleLoaderInterface::loadBackgroundImage(PuzzleBase &puzzle, QString filename)
{
    QString path = m_BackgroundImagesFolder;
    path.append("/").append(filename);

    QDir dir;
    if(dir.exists(dir.absolutePath().append(path)))
    {
        puzzle.m_BackgroundImage = QPixmap(dir.absolutePath().append(path));

        return true;
    }

    return false;
}
