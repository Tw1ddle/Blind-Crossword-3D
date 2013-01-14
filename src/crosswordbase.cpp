#include "crosswordbase.h"

#include <assert.h>

#include <QMessageBox>
#include <QDir>
#include <QColor>

#include "utilities.h"
#include "crosswordstatus.h"


CrosswordBase::CrosswordBase() : m_Loaded(false), m_FileFormatVersion(0.0f)
{
}

//!
//! Converts from uivec3 (x,y,z) coordinate to a single number index into the crossword grid.
//!
unsigned int CrosswordBase::toGridIndex(uivec3 index) const
{
    return index.getX() + getGrid().getDimensions().getX() * index.getY() + getGrid().getDimensions().getY() * getGrid().getDimensions().getX() * index.getZ();
}

//!
//! Gets all the crossword entries who share a letter with the crossword entry identified by the supplied crossword entry id (day number)
//!
std::vector<CrosswordEntry> CrosswordBase::getIntersectingCrosswordEntries(unsigned int crosswordEntryId) const
{
    std::vector<CrosswordEntry> entries;

    for(unsigned int i = 0; i < getEntries().size(); i++)
    {
        Word word = getEntries().at(crosswordEntryId).getGuess();
        if(getEntries().at(i).intersectsWord((&word)))
        {
            entries.push_back(m_Entries.at(i));
        }
    }

    return entries;
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

const std::vector<std::pair<uivec3, QString> >& CrosswordBase::getHighlights() const
{
    return m_Highlights;
}

const std::vector<CrosswordEntry>& CrosswordBase::getEntries() const
{
    return m_Entries;
}
//!
//! Resets the crossword file.
//! \warning This might cause memory leaks if this class is changed to dynamically allocate memory in the constructor. Or, maybe if a derived class is created and then calls this method. Beware.
//!
void CrosswordBase::clear()
{
    *this = CrosswordBase();
}

//!
//! Calculates the score for the crossword based on the number of correct guesses
//!
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

//!
//! Removes all the incorrect answers in a crossword
//!
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

QString CrosswordBase::getScore() const
{
    if(m_Type != CrosswordStatus::WITHOUT_ANSWERS)
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
                append(m_Notes);
    }
    else
    {
        return QString("There is no crossword loaded.");
    }
}

QString CrosswordBase::getTitle() const
{
    return m_Title;
}

FileFormats::FORMAT CrosswordBase::getFormat() const
{
    return m_FileFormat;
}

CrosswordStatus::CROSSWORD_TYPE CrosswordBase::getType() const
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

    if(Utilities::existsFile(dir.absolutePath().append(path)))
    {
        m_BackgroundImage = QPixmap(dir.absolutePath().append(path));

        return true;
    }

    return false;
}
