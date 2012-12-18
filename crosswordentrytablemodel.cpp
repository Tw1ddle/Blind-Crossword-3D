#include "crosswordentrytablemodel.h"

#include <QMessageBox>
#include <QString>
#include <assert.h>

namespace CrosswordEntryTableHeader
{
    const unsigned int identifierColumnId = 0;
    const unsigned int entryColumnId = 1;
    const unsigned int wordColumnId = 2;
    const unsigned int clueColumnId = 3;
    const unsigned int wordLengthColumnId = 4;
}

const QString identifierColumnHeader = "Identifier";
const QString entryColumnHeader = "Entry";
const QString wordColumnHeader = "Word";
const QString clueColumnHeader = "Clue";
const QString wordLengthColumnHeader = "Lengths";

CrosswordEntryTableModel::CrosswordEntryTableModel(const CrosswordBase& puzzle, std::vector<CrosswordEntry>& refCrosswordEntries, QObject *parent) :
    QAbstractTableModel(parent), m_RefPuzzle(puzzle), m_RefCrosswordEntries(refCrosswordEntries), m_RefWorkingGrid(puzzle.getGrid())
{
}

int CrosswordEntryTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_RefCrosswordEntries.size();
}

int CrosswordEntryTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return 5;
}

QVariant CrosswordEntryTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= m_RefCrosswordEntries.size() || index.row() < 0)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if(index.column() == CrosswordEntryTableHeader::identifierColumnId)
        {
            return m_RefCrosswordEntries.at(index.row()).getIdentifier().toUInt();
        }
        if (index.column() == CrosswordEntryTableHeader::entryColumnId)
        {
            QString entryString = m_RefCrosswordEntries.at(index.row()).getEntryName();
            QString entryDirectionName = m_RefCrosswordEntries.at(index.row()).getDirection();
            QString entry = entryString.append(QString(" ")).append(entryDirectionName);

            return entry;
        }
        else if (index.column() == CrosswordEntryTableHeader::wordColumnId)
        {
            return m_RefCrosswordEntries.at(index.row()).getGuess().getString();
        }
        else if(index.column() == CrosswordEntryTableHeader::clueColumnId)
        {
            return m_RefCrosswordEntries.at(index.row()).getClue();
        }
        else if(index.column() == CrosswordEntryTableHeader::wordLengthColumnId)
        {
            return m_RefCrosswordEntries.at(index.row()).getSolutionComponentLengths();
        }
    }
    return QVariant();
}

QVariant CrosswordEntryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
                return identifierColumnHeader;

            case 1:
                return entryColumnHeader;

            case 2:
                return wordColumnHeader;

            case 3:
                return clueColumnHeader;

            case 4:
                return wordLengthColumnHeader;

            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool CrosswordEntryTableModel::existsConflictingWords(QString word, QModelIndex index)
{
    CrosswordEntry currentEntry = m_RefCrosswordEntries.at(index.row());

    bool conflict = false;
    for(int i = 0; i < word.size(); i++)
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

void CrosswordEntryTableModel::crosswordEntriesChanged()
{
    beginResetModel();
    endResetModel();
}

void CrosswordEntryTableModel::amendGuess(QString word, QModelIndex index)
{
    if(m_RefPuzzle.getPuzzleType() == CrosswordTypes::WITHOUT_ANSWERS)
    {
        emit guessAmendationRequestRejected();

        return;
    }
    else
    {
        QString amendedGuess;
        QString removedLetters;
        QString guess = m_RefCrosswordEntries.at(index.row()).getGuess().getString();
        QString solution = m_RefCrosswordEntries.at(index.row()).getSolution();

        assert(guess.size() == solution.size());
        assert(guess.size() == word.size());
        assert(word.size() == solution.size());

        for(int i = 0; i < guess.size(); i++)
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

        m_RefCrosswordEntries.at(index.row()).setGuess(amendedGuess);

        int rows = rowCount(QModelIndex()) - 1;
        int columns = columnCount(QModelIndex()) - 1;
        emit dataChanged(this->index(0, 0), this->index(rows, columns));

        emit guessAmended(removedLetters);
    }
}

void CrosswordEntryTableModel::enterGuess(QString word, QModelIndex index)
{
    if(existsConflictingWords(word, index))
    {
        emit conflictingWordError();
        return;
    }

    m_RefCrosswordEntries.at(index.row()).setGuess(word);

    // Pointer data members in the data source w/ rows affecting other rows means we have to update the whole table when a guess is amended or removed (determining which rows are changed otherwise is inconvenient)
    int rows = rowCount(QModelIndex()) - 1;
    int columns = columnCount(QModelIndex()) - 1;
    emit dataChanged(this->index(0, 0), this->index(rows, columns));

    emit guessValidated(word);
}

void CrosswordEntryTableModel::eraseGuess(QModelIndex index)
{
    m_RefCrosswordEntries.at(index.row()).resetGuess();

    // Pointer data members in the data source w/ rows affecting other rows means we have to update the whole table when a guess is amended or removed (determining which rows are changed otherwise is inconvenient)
    int rows = rowCount(QModelIndex()) - 1;
    int columns = columnCount(QModelIndex()) - 1;
    emit dataChanged(this->index(0, 0), this->index(rows, columns));

    emit guessErased();
}

void CrosswordEntryTableModel::tableViewSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED(previous);

    CrosswordEntry newCrosswordEntry = m_RefCrosswordEntries.at(current.row());

    emit crosswordEntrySelectionChanged(newCrosswordEntry);
}
