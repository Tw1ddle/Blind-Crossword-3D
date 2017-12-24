#include "cluereader.h"

//!
//! Assigns the character index of the text to begin at, and the separator used to break the text into pieces.
//!
ClueReader::ClueReader() : m_Index(0u), m_Separator(QChar(Qt::Key_Space))
{
}

QString ClueReader::advanceWord()
{
    QStringList words = m_Text.split(m_Separator, QString::SkipEmptyParts);

    if(m_Index >= static_cast<std::size_t>(words.size()) - 1)
    {
        m_Index = 0;
    }
    else
    {
        m_Index++;
    }

    return getWord();
}

QString ClueReader::getWord() const
{
    QStringList words = m_Text.split(m_Separator, QString::SkipEmptyParts);

    if(m_Index < static_cast<std::size_t>(words.size()))
    {
        return words.at(m_Index);
    }
    else
    {
        return "No clue selected";
    }
}

void ClueReader::setText(CrosswordEntry entry)
{
    m_Index = 0;
    m_Text = entry.getClue();
}
