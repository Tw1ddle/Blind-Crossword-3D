#ifndef PUZZLE3D_H
#define PUZZLE3D_H

#include <QString>
#include <QPixmap>
#include <QFileInfo>
#include <vector>

#include "crosswordentry3d.h"
#include "puzzleloader.h"
#include "lettergrid.h"

class BCrossword3D : public QObject
{
    friend class PuzzleLoader;

public:
    BCrossword3D();

    LetterGrid& getRefGrid();
    std::vector<CrosswordEntry3D>& getRefCrosswordEntries();
    QPixmap& getRefPuzzleBackgroundImage();

    void setDimensions(uivec3 dimensions);

    QString getPuzzleTitle() const;
    QString getInformationString() const;
    QString getPuzzleThemePhrase() const;

    QString getScoreString() const;
    FileFormats::FORMAT getPuzzleFormat() const;

    unsigned int removeIncorrectEntries();

private:
    void clear();
    unsigned int scoreSolution() const;

    LetterGrid m_Grid;
    std::vector<CrosswordEntry3D> m_CrosswordEntries;
    QPixmap m_BackgroundImage;

    QString m_PuzzleTitle;
    QString m_AuthorTitle;
    QString m_PuzzleType;
    QString m_PuzzleThemePhrase;
    QString m_PuzzleNotes;

    bool m_CrosswordLoaded;
    FileFormats::FORMAT m_CrosswordFileFormat;
};

#endif // PUZZLE3D_H
