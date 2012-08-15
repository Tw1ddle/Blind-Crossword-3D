#include "puzzle3d.h"

#include <QMessageBox>

BCrossword3D::BCrossword3D() : m_CrosswordLoaded(false)
{
}

void BCrossword3D::setDimensions(uivec3 dimensions)
{
    m_Grid.setDimensions(dimensions);
}

LetterGrid &BCrossword3D::getRefGrid()
{
    return m_Grid;
}

std::vector<CrosswordEntry3D> &BCrossword3D::getRefCrosswordEntries()
{
    return m_CrosswordEntries;
}

void BCrossword3D::clear()
{
    m_PuzzleTitle.clear();
    m_AuthorTitle.clear();
    m_PuzzleType.clear();
    m_Grid.clear();
    m_CrosswordEntries.clear();
    m_CrosswordFileFormat.clear();
    m_CrosswordLoaded = false;
}

unsigned int BCrossword3D::scoreSolution() const
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

unsigned int BCrossword3D::removeIncorrectEntries()
{
    unsigned int entriesRemoved = 0;

    for(unsigned int i = 0; i < m_CrosswordEntries.size(); i++)
    {
        if(!m_CrosswordEntries.at(i).isGuessCorrect())
        {
            m_CrosswordEntries.at(i).resetGuessString();
            entriesRemoved++;
        }
    }
    return entriesRemoved;
}

QString BCrossword3D::getScoreString() const
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

QString BCrossword3D::getInformationString() const
{
    if(m_CrosswordLoaded)
    {
        return QString(tr("Crossword title: ")).append(m_PuzzleTitle).append(". \n").
                append(tr("Author: ")).append(m_AuthorTitle).append(". \n").
                append(tr("Type: ")).append(m_PuzzleType);
    }
    else
    {
        return QString(tr("There is no crossword loaded."));
    }
}

QString BCrossword3D::getPuzzleTitle() const
{
    return m_PuzzleTitle;
}

FileFormats::FORMAT BCrossword3D::getPuzzleFormat() const
{
    return m_CrosswordFileFormat;
}
