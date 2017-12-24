/*! \brief Models a crossword.
 *  Models a crossword. It consists of metadata, the crossword grid, and the entries (clues and associated information).
 *  The model has minimal functionality itself - it is a data source for other classes.
 *
 *  Loading and unloading of data is delegated to loader friend classes.
 */

#pragma once

#include <QString>
#include <QPixmap>
#include <vector>

#include "crosswordentry.h"
#include "crosswordgrid.h"

#include "crosswordfileformats.h"
#include "crosswordstatus.h"

const QString m_BackgroundImagesFolder = "/Backgrounds";

class CrosswordBase
{
    friend class CrosswordLoader;

    friend class XWC3DLoader;
    friend class XWCLoader;

public:
    CrosswordBase();

    unsigned int toGridIndex(uivec3 index) const;

    std::vector<CrosswordEntry> getIntersectingCrosswordEntries(unsigned int crosswordEntryId) const;
    unsigned int removeIncorrectEntries();

    QString getTitle() const;
    QString getInformation() const;
    QString getScore() const;
    FileFormats::FORMAT getFormat() const;
    CrosswordStatus::CROSSWORD_TYPE getType() const;
    bool isComplete() const;

    const GridData& getGrid() const;
    const std::vector<CrosswordEntry>& getEntries() const;
    const QPixmap& getBackgroundImage() const;
    const std::vector<std::pair<uivec3, QString> >& getHighlights() const;

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
    std::vector<std::pair<uivec3, QString> > m_Highlights; //! Uses the W3 SVG colour scheme, see: http://www.w3.org/TR/SVG/types.html#ColorKeywords
    QString m_Notes;

    QPixmap m_BackgroundImage;
    QString m_BackgroundImageFilename;
    GridData m_Grid;
    std::vector<CrosswordEntry> m_Entries;

    bool m_Loaded;
    FileFormats::FORMAT m_FileFormat;
    float m_FileFormatVersion;
};
