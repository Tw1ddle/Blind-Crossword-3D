/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CROSSWORDBASE_H
#define CROSSWORDBASE_H

#include <QString>
#include <QPixmap>
#include <vector>

#include "crosswordentry.h"
#include "crosswordgrid.h"

#include "fileformats.h"
#include "crosswordtypes.h"

const QString m_BackgroundImagesFolder = "/Backgrounds";

class CrosswordBase
{
    friend class CrosswordLoader;

    friend class CrosswordLoaderInterface;
    friend class XWC3DLoader;
    friend class XWCLoader;

public:
    CrosswordBase();

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

    const GridData& getGrid() const;
    const std::vector<CrosswordEntry>& getCrosswordEntries() const;
    const QPixmap& getPuzzleBackgroundImage() const;
    const std::vector<uivec3>& getThemePhraseCoordinates() const;

    std::vector<CrosswordEntry>& getRefCrosswordEntries();

protected:
    GridData& getRefGrid();
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
    QString m_BackgroundImageFilename;
    GridData m_Grid;
    std::vector<CrosswordEntry> m_CrosswordEntries;

    bool m_CrosswordLoaded;
    FileFormats::FORMAT m_CrosswordFileFormat;
    float m_FileFormatVersion;
};

#endif // CROSSWORDBASE_H
