/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

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
#include "crosswordtypes.h"

class GraphicsGridItem;

const QString m_BackgroundImagesFolder = "/Backgrounds";

class PuzzleBase
{
    friend class PuzzleLoader;

    friend class PuzzleLoaderInterface;
    friend class XWC3DLoader;
    friend class XWCLoader;
    friend class XWCR3DLoader;

public:
    PuzzleBase();

    unsigned int toGridIndex(uivec3 index) const;

    std::vector<unsigned int> getIntersectingCrosswordEntryIds(unsigned int crosswordEntryId) const;
    unsigned int removeIncorrectEntries();

    QString getPuzzleTitle() const;
    QString getInformationString() const;
    QString getPuzzleThemePhrase() const;
    QString getScoreString() const;
    FileFormats::FORMAT getPuzzleFormat() const;
    CrosswordTypes::CROSSWORD_TYPE getPuzzleType() const;
    bool isComplete() const;

    const LetterGrid& getGrid() const;
    const std::vector<CrosswordEntry3D>& getCrosswordEntries() const;
    const QPixmap& getPuzzleBackgroundImage() const;
    const std::vector<uivec3>& getThemePhraseCoordinates() const;

    std::vector<CrosswordEntry3D>& getRefCrosswordEntries();

protected:
    LetterGrid& getRefGrid();
    void setDimensions(uivec3 dimensions);

private:
    void clear();
    unsigned int scoreSolution() const;
    bool loadBackgroundImage(QString filename);

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
    unsigned int m_FileFormatVersion;
};

#endif // PUZZLEBASE_H
