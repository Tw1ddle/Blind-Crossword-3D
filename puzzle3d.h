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
    friend class XWC3DLoader;
    friend class XWCLoader;

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
    std::vector<unsigned int> getIntersectingCrosswordEntryIds(unsigned int crosswordEntryId);

private:
    void clear();
    unsigned int scoreSolution() const;

    bool loadBackgroundImage(QString filename);

    static const QString m_BackgroundImagesFolder;

    QString m_PuzzleTitle;
    QString m_AuthorTitle;
    QString m_PuzzleType;
    QString m_PuzzleThemePhrase;
    QString m_PuzzleNotes;
    std::vector<uivec3> m_ThemePhraseCoordinates;
    QPixmap m_BackgroundImage;

    LetterGrid m_Grid;
    std::vector<CrosswordEntry3D> m_CrosswordEntries;

    bool m_CrosswordLoaded;
    FileFormats::FORMAT m_CrosswordFileFormat;
};

#endif // PUZZLE3D_H
