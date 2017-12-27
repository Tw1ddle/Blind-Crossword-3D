#include "crossword/crosswordbase.h"

#include <cassert>

#include <QColor>
#include <QDir>
#include <QMessageBox>

#include "assets/assetpaths.h"
#include "crossword/crosswordstatus.h"
#include "util/util.h"

namespace crossword {

CrosswordBase::CrosswordBase() : m_loaded(false), m_fileFormatVersion(0.0f)
{
}

unsigned int CrosswordBase::toGridIndex(util::uivec3 index) const
{
    return index.getX() + getGrid().getDimensions().getX() * index.getY() +
           getGrid().getDimensions().getY() * getGrid().getDimensions().getX() * index.getZ();
}

std::vector<CrosswordEntry> CrosswordBase::getIntersectingCrosswordEntries(
    unsigned int crosswordEntryId) const
{
    std::vector<CrosswordEntry> entries;

    for (unsigned int i = 0; i < getEntries().size(); i++) {
        Word word = getEntries().at(crosswordEntryId).getGuess();

        if (getEntries().at(i).intersectsWord((&word))) {
            entries.push_back(m_entries.at(i));
        }
    }

    return entries;
}

void CrosswordBase::setDimensions(util::uivec3 dimensions)
{
    m_grid.setDimensions(dimensions);
}

GridData& CrosswordBase::getRefGrid()
{
    return m_grid;
}

std::vector<CrosswordEntry>& CrosswordBase::getRefEntries()
{
    return m_entries;
}

const GridData& CrosswordBase::getGrid() const
{
    return m_grid;
}

const QPixmap& CrosswordBase::getBackgroundImage() const
{
    return m_backgroundImage;
}

const std::vector<std::pair<util::uivec3, QString> >& CrosswordBase::getHighlights() const
{
    return m_highlights;
}

const std::vector<CrosswordEntry>& CrosswordBase::getEntries() const
{
    return m_entries;
}

void CrosswordBase::clear()
{
    *this = CrosswordBase();
}

unsigned int CrosswordBase::scoreSolution() const
{
    unsigned int score = 0;

    for (unsigned int i = 0; i < m_entries.size(); i++) {
        if (m_entries.at(i).isGuessCorrect()) {
            score++;
        }
    }

    return score;
}

unsigned int CrosswordBase::removeIncorrectEntries()
{
    unsigned int entriesRemoved = 0;

    for (unsigned int i = 0; i < m_entries.size(); i++) {
        if (!m_entries.at(i).isGuessCorrect()) {
            m_entries.at(i).resetGuess();
            entriesRemoved++;
        }
    }

    return entriesRemoved;
}

QString CrosswordBase::getScore() const
{
    if (m_type != crossword::WITHOUT_ANSWERS) {
        if (m_loaded) {
            return QString("The current score for this crossword is: ").
                   append(QString::number(scoreSolution())).
                   append(" out of ").
                   append(QString::number(m_entries.size())).append(". ");
        } else {
            return QString("There is no crossword loaded, so one cannot be scored.");
        }
    } else {
        return QString("This crossword does not have answers included, so cannot be scored.");
    }
}

QString CrosswordBase::getInformation() const
{
    if (m_loaded) {
        return QString("Crossword title: ").append(m_title).append(". \n").
               append("Author: ").append(m_authors).append(". \n").
               append("Type: ").append(m_type).append(". \n").
               append(m_notes);
    } else {
        return QString("There is no crossword loaded.");
    }
}

QString CrosswordBase::getTitle() const
{
    return m_title;
}

fileformat::FORMAT CrosswordBase::getFormat() const
{
    return m_fileFormat;
}

crossword::CROSSWORD_TYPE CrosswordBase::getType() const
{
    return m_type;
}

bool CrosswordBase::isComplete() const
{
    return scoreSolution() == m_entries.size();
}

bool CrosswordBase::loadBackgroundImage(QString filename)
{
    const QString path = assets::getEmbeddedBackgroundsFolderPath() + "/" + filename;

    if (!QFile::exists(path)) {
        return false;
    }

    m_backgroundImage = QPixmap(path);
    return true;
}

}
