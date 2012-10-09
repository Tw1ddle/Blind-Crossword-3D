#ifndef PUZZLEBASE_H
#define PUZZLEBASE_H

#include <QString>
#include <QPixmap>
#include <QFileInfo>
#include <vector>

#include "singleton.h"
#include "crosswordentry3d.h"
#include "puzzleloader.h"
#include "lettergrid.h"
#include "fileformats.h"

class PuzzleBase : public QObject
{
    friend class PuzzleLoaderInterface;
    friend class XWC3DLoader;
    friend class XWCLoader;
    friend class XWCR3DLoader;
    friend class PuzzleLoader;

public:
    PuzzleBase();

    virtual unsigned int toGridIndex(uivec3 index) const = 0;

    virtual std::vector<unsigned int> getIntersectingCrosswordEntryIds(unsigned int crosswordEntryId) const;
    unsigned int removeIncorrectEntries();

    QString getPuzzleTitle() const;
    QString getInformationString() const;
    QString getPuzzleThemePhrase() const;
    QString getScoreString() const;
    FileFormats::FORMAT getPuzzleFormat() const;

    const LetterGrid& getGrid() const;
    const std::vector<CrosswordEntry3D>& getCrosswordEntries() const;
    const QPixmap& getPuzzleBackgroundImage() const;

    std::vector<CrosswordEntry3D>& getRefCrosswordEntries();

protected:
    LetterGrid& getRefGrid();
    void setDimensions(uivec3 dimensions);

private:
    void clear();
    unsigned int scoreSolution() const;

    QString m_PuzzleTitle;
    QString m_AuthorTitle;
    QString m_PuzzleType;
    QString m_PuzzleThemePhrase;
    std::vector<uivec3> m_ThemePhraseCoordinates;
    QString m_PuzzleNotes;

    QPixmap m_BackgroundImage;
    LetterGrid m_Grid;
    std::vector<CrosswordEntry3D> m_CrosswordEntries;

    bool m_CrosswordLoaded;
    FileFormats::FORMAT m_CrosswordFileFormat;
};

#endif // PUZZLEBASE_H
