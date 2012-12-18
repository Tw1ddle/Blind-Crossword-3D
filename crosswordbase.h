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

#include "crosswordfileformats.h"
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

    std::vector<CrosswordEntry> getIntersectingCrosswordEntries(unsigned int crosswordEntryId) const;
    unsigned int removeIncorrectEntries();

    QString getTitle() const;
    QString getInformation() const;
    QString getThemePhrase() const;
    QString getScore() const;
    FileFormats::FORMAT getFormat() const;
    CrosswordTypes::CROSSWORD_TYPE getType() const;
    bool isComplete() const;

    const GridData& getGrid() const;
    const std::vector<CrosswordEntry>& getEntries() const;
    const QPixmap& getBackgroundImage() const;
    const std::vector<uivec3>& getThemePhraseCoordinates() const;

    std::vector<CrosswordEntry>& getRefEntries();

protected:
    GridData& getRefGrid();
    void setDimensions(uivec3 dimensions);

private:
    void clear();
    unsigned int scoreSolution() const;
    bool loadBackgroundImage(QString filename);

    QString m_Title;
    QString m_Authors;
    QString m_Type;
    QString m_ThemePhrase;
    std::vector<uivec3> m_ThemePhraseCoordinates;
    QString m_Notes;

    QPixmap m_BackgroundImage;
    QString m_BackgroundImageFilename;
    GridData m_Grid;
    std::vector<CrosswordEntry> m_Entries;

    bool m_Loaded;
    FileFormats::FORMAT m_FileFormat;
    float m_FileFormatVersion;
};

#endif // CROSSWORDBASE_H
