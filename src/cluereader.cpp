#include "cluereader.h"

//!
//! Assigns the character index of the text to begin at, and the separator used to break the text into pieces.
//!
ClueReader::ClueReader() : index(0u), separator(QChar(Qt::Key_Space))
{
}

QString ClueReader::advanceWord()
{
    QStringList words = text.split(separator, QString::SkipEmptyParts);

    if(index >= words.length() - 1)
    {
        index = 0;
    }
    else
    {
        index++;
    }

    return getWord();
}

QString ClueReader::getWord()
{
    QStringList words = text.split(separator, QString::SkipEmptyParts);

    if(!words.at(index).isNull())
    {
        return words.at(index);
    }
    else
    {
        return "No clue selected";
    }
}

void ClueReader::setText(CrosswordEntry entry)
{
    index = 0;
    text = entry.getClue();
}
