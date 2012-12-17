#include "crosswordbase.h"

#include <assert.h>

#include <QMessageBox>
#include <QDir>

#include "crosswordtypes.h"

CrosswordBase::CrosswordBase() : m_CrosswordLoaded(false), m_FileFormatVersion(0.0f)
{
}

unsigned int CrosswordBase::toGridIndex(uivec3 index) const
{
    if(m_CrosswordFileFormat == FileFormats::XWC || m_CrosswordFileFormat == FileFormats::XWC3D)
    {
        return index.getX() + getGrid().getDimensions().getX() * index.getY() + getGrid().getDimensions().getY() * getGrid().getDimensions().getX() * index.getZ();
    }
    else if(m_CrosswordFileFormat == FileFormats::XWCR3D)
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

std::vector<CrosswordEntry>& CrosswordBase::getRefCrosswordEntries()
{
    return m_CrosswordEntries;
}

const GridData& CrosswordBase::getGrid() const
{
    return m_Grid;
}

const QPixmap& CrosswordBase::getPuzzleBackgroundImage() const
{
    return m_BackgroundImage;
}

const std::vector<uivec3>& CrosswordBase::getThemePhraseCoordinates() const
{
    return m_ThemePhraseCoordinates;
}

const std::vector<CrosswordEntry>& CrosswordBase::getCrosswordEntries() const
{
    return m_CrosswordEntries;
}

void CrosswordBase::clear()
{
    m_PuzzleTitle.clear();
    m_AuthorTitle.clear();
    m_PuzzleType.clear();
    m_PuzzleThemePhrase.clear();
    m_PuzzleNotes.clear();
    m_ThemePhraseCoordinates.clear();
    m_Grid.clear();
    m_CrosswordEntries.clear();
    m_CrosswordFileFormat.clear();
    m_FileFormatVersion = 0.0f;

    m_CrosswordLoaded = false;

    m_BackgroundImage.detach();
}

unsigned int CrosswordBase::scoreSolution() const
{
    unsigned int score = 0;

    for(unsigned int i = 0; i < m_CrosswordEntries.size(); i++)
    {
        if(m_CrosswordEntries.at(i).isGuessCorrect())
        {
            score++;
        }
    }
    return score;
}

unsigned int CrosswordBase::removeIncorrectEntries()
{
    unsigned int entriesRemoved = 0;

    for(unsigned int i = 0; i < m_CrosswordEntries.size(); i++)
    {
        if(!m_CrosswordEntries.at(i).isGuessCorrect())
        {
            m_CrosswordEntries.at(i).resetGuess();
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

QString CrosswordBase::getScoreString() const
{
    if(m_PuzzleType != CrosswordTypes::WITHOUT_ANSWERS)
    {
        if(m_CrosswordLoaded)
        {
            return QString("The current score for this crossword is: ").
                    append(QString::number(scoreSolution())).
                    append(" out of ").
                    append(QString::number(m_CrosswordEntries.size())).append(". ");
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

QString CrosswordBase::getInformationString() const
{
    if(m_CrosswordLoaded)
    {
        return QString("Crossword title: ").append(m_PuzzleTitle).append(". \n").
                append("Author: ").append(m_AuthorTitle).append(". \n").
                append("Type: ").append(m_PuzzleType).append(". \n").
                append("Theme phrase: ").append(m_PuzzleThemePhrase);
    }
    else
    {
        return QString("There is no crossword loaded.");
    }
}

QString CrosswordBase::getPuzzleThemePhrase() const
{
    return m_PuzzleThemePhrase;
}

QString CrosswordBase::getPuzzleTitle() const
{
    return m_PuzzleTitle;
}

FileFormats::FORMAT CrosswordBase::getPuzzleFormat() const
{
    return m_CrosswordFileFormat;
}

CrosswordTypes::CROSSWORD_TYPE CrosswordBase::getPuzzleType() const
{
    return m_PuzzleType;
}

bool CrosswordBase::isComplete() const
{
    return (scoreSolution() == m_CrosswordEntries.size());
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
