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

#include "crossword/crosswordentry.h"
#include "crossword/crosswordgrid.h"
#include "crossword/crosswordfileformats.h"
#include "crossword/crosswordstatus.h"

const QString BACKGROUND_IMAGES_FOLDER = ":/backgrounds";

class CrosswordBase {
    friend class CrosswordLoader;
    friend class XWC3DLoader;
    friend class XWCLoader;

  public:
    CrosswordBase();

    //!
    //! Converts from uivec3 (x,y,z) coordinate to a single number index into the crossword grid.
    //!
    unsigned int toGridIndex(uivec3 index) const;

    //!
    //! Gets all the crossword entries who share a letter with the crossword entry identified by the supplied crossword entry id (day number)
    //!
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

    QString m_title;
    QString m_authors;
    QString m_type;
    std::vector<std::pair<uivec3, QString> >
    m_highlights; //! Uses the W3 SVG colour scheme, see: http://www.w3.org/TR/SVG/types.html#ColorKeywords
    QString m_notes;

    QPixmap m_backgroundImage;
    QString m_backgroundImageFilename;
    GridData m_grid;
    std::vector<CrosswordEntry> m_entries;

    bool m_loaded;
    FileFormats::FORMAT m_fileFormat;
    float m_fileFormatVersion;
};
