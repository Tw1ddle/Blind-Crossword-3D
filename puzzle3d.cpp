#include "puzzle3d.h"

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

void BCrossword3D::clearPuzzle()
{
    m_PuzzleTitle.clear();
    m_AuthorTitle.clear();
    m_PuzzleType.clear();
    m_Grid.clear();
    m_CrosswordEntries.clear();
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

void BCrossword3D::showInformation(QWidget* parent)
{
    if(m_CrosswordLoaded)
    {
        QMessageBox::information(parent, tr("Crossword properties"),
                                 QString("Title: ").append(m_PuzzleTitle).append("\n").
                                 append(QString("Author: ")).append(QString(m_AuthorTitle)).append("\n").
                                 append(QString("Crossword Type: ")).append(QString(m_PuzzleType)));
    }
    else
    {
        QMessageBox::information(parent, tr("Crossword properties"),
                                 tr("No crossword loaded!"));
    }

}
