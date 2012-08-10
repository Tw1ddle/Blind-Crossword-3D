#include "wordtablemodel.h"

#include <QMessageBox>
#include <QString>
#include <assert.h>

const QString wordColumnHeader = "Word";
const QString clueColumnHeader = "Clue";
const QString entryNumberColumnHeader = "Entry";
const QString wordLengthColumnHeader = "Word Lengths";


WordTableModel::WordTableModel(std::vector<CrosswordEntry3D>* refCrosswordEntries, LetterGrid* refWorkingGrid, QObject *parent) :
    QAbstractTableModel(parent)
{
    m_RefWorkingGrid = refWorkingGrid;
    m_RefCrosswordEntries = refCrosswordEntries;
}

int WordTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_RefCrosswordEntries->size();
}

int WordTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return 4;
}

QVariant WordTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= m_RefCrosswordEntries->size() || index.row() < 0)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            unsigned int entryNum = m_RefCrosswordEntries->at(index.row()).getEntryNumber();
            QString entryDirectionName = m_RefCrosswordEntries->at(index.row()).getDirection().getDirectionName();
            QString entry = QString::number(entryNum).append(QString(" ")).append(entryDirectionName);

            return entry;
        }
        else if (index.column() == 1)
        {
            return m_RefCrosswordEntries->at(index.row()).getGuess().getString();
        }
        else if(index.column() == 2)
        {
            return m_RefCrosswordEntries->at(index.row()).getClue().getString();
        }
        else if(index.column() == 3)
        {
            return m_RefCrosswordEntries->at(index.row()).getClue().getString();
        }
    }
    return QVariant();
}

QVariant WordTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return entryNumberColumnHeader;

            case 1:
                return wordColumnHeader;

            case 2:
                return clueColumnHeader;

            case 3:
                return wordLengthColumnHeader;

            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool WordTableModel::existsConflictingWords(QString word, QModelIndex index)
{
    CrosswordEntry3D currentEntry = m_RefCrosswordEntries->at(index.row());

    bool conflict = false;
    for(unsigned int i = 0; i < word.size(); i++)
    {
        if(currentEntry.getGuess().getString().at(i) == word.at(i) || currentEntry.getGuess().getString().at(i) == QChar(Qt::Key_Period) || word.at(i) == QChar(Qt::Key_Period))
        {
        }
        else
        {
            conflict = true;
        }
    }

    return conflict;
}

void WordTableModel::crosswordEntriesChanged()
{
    beginResetModel();
    endResetModel();
}

void WordTableModel::amendGuess(QModelIndex index)
{
    QString amendedGuess;
    QString removedLetters;
    QString guess = m_RefCrosswordEntries->at(index.row()).getGuess().getString();
    QString solution = m_RefCrosswordEntries->at(index.row()).getSolution();

    assert(guess.size() == solution.size());

    for(unsigned int i = 0; i < guess.size(); i++)
    {
        if(guess.at(i) == solution.at(i))
        {
            amendedGuess.push_back(solution.at(i));
        }
        else
        {
            amendedGuess.push_back(Qt::Key_Period);

            if(guess.at(i) != Qt::Key_Period)
            {
                removedLetters.push_back(guess.at(i));
            }
        }
    }

    m_RefCrosswordEntries->at(index.row()).setGuessString(amendedGuess);

    beginResetModel();
    endResetModel();

    emit guessAmended(removedLetters);
}

void WordTableModel::enterGuess(QString word, QModelIndex index)
{
    if(existsConflictingWords(word, index))
    {
        emit conflictingWordError();
        return;
    }

    m_RefCrosswordEntries->at(index.row()).setGuessString(word);

    beginResetModel();
    endResetModel();

    emit guessValidated(word);
}
