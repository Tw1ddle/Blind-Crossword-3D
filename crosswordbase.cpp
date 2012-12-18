#include "crosswordbase.h"

#include <assert.h>

#include <QMessageBox>
#include <QDir>

#include "crosswordtypes.h"

CrosswordBase::CrosswordBase() : m_Loaded(false), m_FileFormatVersion(0.0f)
{
}

unsigned int CrosswordBase::toGridIndex(uivec3 index) const
{
    if(m_FileFormat == FileFormats::XWC || m_FileFormat == FileFormats::XWC3D)
    {
        return index.getX() + getGrid().getDimensions().getX() * index.getY() + getGrid().getDimensions().getY() * getGrid().getDimensions().getX() * index.getZ();
    }
    else if(m_FileFormat == FileFormats::XWCR3D)
    {
        if(index.getY() == 0) // disc center
        {
            return index.getZ() + index.getZ() * (getGrid().getDimensions().getY() - 1) * getGrid().getDimensions().getX();
        }
        else
        {
            return 1
                    + index.getZ()
                    + index.getX()
                    + (index.getY() - 1) * getGrid().getDimensions().getX()
                    + index.getZ() * (getGrid().getDimensions().getY() - 1) * getGrid().getDimensions().getX();
        }
    }

    assert(false);

    return 0;
}

void CrosswordBase::setDimensions(uivec3 dimensions)
{
    m_Grid.setDimensions(dimensions);
}

GridData &CrosswordBase::getRefGrid()
{
    return m_Grid;
}

std::vector<CrosswordEntry>& CrosswordBase::getRefEntries()
{
    return m_Entries;
}

const GridData& CrosswordBase::getGrid() const
{
    return m_Grid;
}

const QPixmap& CrosswordBase::getBackgroundImage() const
{
    return m_BackgroundImage;
}

const std::vector<uivec3>& CrosswordBase::getThemePhraseCoordinates() const
{
    return m_ThemePhraseCoordinates;
}

const std::vector<CrosswordEntry>& CrosswordBase::getEntries() const
{
    return m_Entries;
}

void CrosswordBase::clear()
{
    m_Title.clear();
    m_Authors.clear();
    m_Type.clear();
    m_ThemePhrase.clear();
    m_Notes.clear();
    m_ThemePhraseCoordinates.clear();
    m_Grid.clear();
    m_Entries.clear();
    m_FileFormat.clear();
    m_FileFormatVersion = 0.0f;

    m_Loaded = false;

    m_BackgroundImage.detach();
}

unsigned int CrosswordBase::scoreSolution() const
{
    unsigned int score = 0;

    for(unsigned int i = 0; i < m_Entries.size(); i++)
    {
        if(m_Entries.at(i).isGuessCorrect())
        {
            score++;
        }
    }
    return score;
}

unsigned int CrosswordBase::removeIncorrectEntries()
{
    unsigned int entriesRemoved = 0;

    for(unsigned int i = 0; i < m_Entries.size(); i++)
    {
        if(!m_Entries.at(i).isGuessCorrect())
        {
            m_Entries.at(i).resetGuess();
            entriesRemoved++;
        }
    }
    return entriesRemoved;
}

std::vector<unsigned int> CrosswordBase::getIntersectingCrosswordEntryIds(unsigned int crosswordEntryId) const
{
    std::vector<unsigned int> intersectingIds;

    //todo

    return intersectingIds;
}

QString CrosswordBase::getScore() const
{
    if(m_Type != CrosswordTypes::WITHOUT_ANSWERS)
    {
        if(m_Loaded)
        {
            return QString("The current score for this crossword is: ").
                    append(QString::number(scoreSolution())).
                    append(" out of ").
                    append(QString::number(m_Entries.size())).append(". ");
        }
        else
        {
            return QString("There is no crossword loaded, so one cannot be scored.");
        }
    }
    else
    {
        return QString("This crossword does not have answers included, so cannot be scored.");
    }
}

QString CrosswordBase::getInformation() const
{
    if(m_Loaded)
    {
        return QString("Crossword title: ").append(m_Title).append(". \n").
                append("Author: ").append(m_Authors).append(". \n").
                append("Type: ").append(m_Type).append(". \n").
                append("Theme phrase: ").append(m_ThemePhrase);
    }
    else
    {
        return QString("There is no crossword loaded.");
    }
}

QString CrosswordBase::getThemePhrase() const
{
    return m_ThemePhrase;
}

QString CrosswordBase::getTitle() const
{
    return m_Title;
}

FileFormats::FORMAT CrosswordBase::getFormat() const
{
    return m_FileFormat;
}

CrosswordTypes::CROSSWORD_TYPE CrosswordBase::getType() const
{
    return m_Type;
}

bool CrosswordBase::isComplete() const
{
    return (scoreSolution() == m_Entries.size());
}

bool CrosswordBase::loadBackgroundImage(QString filename)
{
    QString path = m_BackgroundImagesFolder;
    path.append("/").append(filename);

    QDir dir;
    if(dir.exists(dir.absolutePath().append(path)))
    {
        m_BackgroundImage = QPixmap(dir.absolutePath().append(path));

        return true;
    }

    return false;
}