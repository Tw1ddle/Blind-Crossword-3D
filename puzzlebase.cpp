#include "puzzlebase.h"

#include <assert.h>

#include <QMessageBox>
#include <QDir>

#include "crosswordtypes.h"

PuzzleBase::PuzzleBase() : m_CrosswordLoaded(false), m_FileFormatVersion(1u)
{
}

unsigned int PuzzleBase::toGridIndex(uivec3 index) const
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
    else
    {
        assert(false);
    }
}

void PuzzleBase::setDimensions(uivec3 dimensions)
{
    m_Grid.setDimensions(dimensions);
}

LetterGrid &PuzzleBase::getRefGrid()
{
    return m_Grid;
}

std::vector<CrosswordEntry3D>& PuzzleBase::getRefCrosswordEntries()
{
    return m_CrosswordEntries;
}

const LetterGrid& PuzzleBase::getGrid() const
{
    return m_Grid;
}

const QPixmap& PuzzleBase::getPuzzleBackgroundImage() const
{
    return m_BackgroundImage;
}

const std::vector<uivec3>& PuzzleBase::getThemePhraseCoordinates() const
{
    return m_ThemePhraseCoordinates;
}

const std::vector<CrosswordEntry3D>& PuzzleBase::getCrosswordEntries() const
{
    return m_CrosswordEntries;
}

void PuzzleBase::clear()
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
    m_BackgroundImage = NULL;

}

unsigned int PuzzleBase::scoreSolution() const
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

unsigned int PuzzleBase::removeIncorrectEntries()
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

std::vector<unsigned int> PuzzleBase::getIntersectingCrosswordEntryIds(unsigned int crosswordEntryId) const
{
    std::vector<unsigned int> intersectingIds;

    //todo

    return intersectingIds;
}

QString PuzzleBase::getScoreString() const
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

QString PuzzleBase::getInformationString() const
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

QString PuzzleBase::getPuzzleTitle() const
{
    return m_PuzzleTitle;
}

QString PuzzleBase::getPuzzleThemePhrase() const
{
    QString themePhrase;

    for(unsigned int i = 0; i < m_ThemePhraseCoordinates.size(); i++)
    {
        themePhrase.append(this->getGrid().getLetterAt(toGridIndex(m_ThemePhraseCoordinates.at(i) - uivec3(1, 1, 1)))->getChar());
    }

    return themePhrase;
}

FileFormats::FORMAT PuzzleBase::getPuzzleFormat() const
{
    return m_CrosswordFileFormat;
}

CrosswordTypes::CROSSWORD_TYPE PuzzleBase::getPuzzleType() const
{
    return m_PuzzleType;
}

bool PuzzleBase::isComplete() const
{
    return (scoreSolution() == m_CrosswordEntries.size());
}
