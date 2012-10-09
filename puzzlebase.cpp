#include "puzzlebase.h"

#include <QMessageBox>
#include <QDir>

PuzzleBase::PuzzleBase() : m_CrosswordLoaded(false)
{
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
    m_CrosswordLoaded = false;
    m_BackgroundImage.detach();
    m_CrosswordFileFormat.clear();
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
    if(m_CrosswordLoaded)
    {
        return QString(tr("The current score for this crossword is: ")).
                append(QString::number(scoreSolution())).
                append(tr(" out of ")).
                append(QString::number(m_CrosswordEntries.size())).append(". ");
    }
    else
    {
        return QString(tr("There is no crossword loaded, so one cannot be scored."));
    }
}

QString PuzzleBase::getInformationString() const
{
    if(m_CrosswordLoaded)
    {
        return QString(tr("Crossword title: ")).append(m_PuzzleTitle).append(". \n").
                append(tr("Author: ")).append(m_AuthorTitle).append(". \n").
                append(tr("Type: ")).append(m_PuzzleType).append(". \n").
                append(tr("Theme phrase: ")).append(m_PuzzleThemePhrase);
    }
    else
    {
        return QString(tr("There is no crossword loaded."));
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
