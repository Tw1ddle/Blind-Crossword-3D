#include "crossword/cluereader.h"

//!
//! Assigns the character index of the text to begin at, and the separator used to break the text into pieces.
//!
ClueReader::ClueReader() : m_index(0u), m_separator(QChar(Qt::Key_Space))
{
}

QString ClueReader::advanceWord()
{
    QStringList words = m_text.split(m_separator, QString::SkipEmptyParts);

    if (m_index >= static_cast<std::size_t>(words.size()) - 1) {
        m_index = 0;
    } else {
        m_index++;
    }

    return getWord();
}

QString ClueReader::getWord() const
{
    QStringList words = m_text.split(m_separator, QString::SkipEmptyParts);

    if (m_index < static_cast<std::size_t>(words.size())) {
        return words.at(m_index);
    } else {
        return "No clue selected";
    }
}

void ClueReader::setText(CrosswordEntry entry)
{
    m_index = 0;
    m_text = entry.getClue();
}
