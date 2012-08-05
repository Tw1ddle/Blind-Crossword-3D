#include "wordtablemodel.h"
#include <QMessageBox>
#include <QString>

const QString wordColumnHeader = "Word";
const QString clueColumnHeader = "Clue";

WordTableModel::WordTableModel(std::vector<CrosswordEntry3D>* refCrosswordEntries, LetterGrid* refWorkingGrid, QObject *parent) :
    QAbstractTableModel(parent)
{
    m_RefWorkingGrid = refWorkingGrid;
    m_RefCrosswordEntries = refCrosswordEntries;
}

int WordTableModel::rowCount(const QModelIndex& parent) const
{
    return m_UserCrosswordEntries.size();
}

int WordTableModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant WordTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= m_UserCrosswordEntries.size() || index.row() < 0)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return m_UserCrosswordEntries.at(index.row()).getGuess();
        }
        else if (index.column() == 1)
        {
            return m_UserCrosswordEntries.at(index.row()).getClue().getString();
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
                return wordColumnHeader;

            case 1:
                return clueColumnHeader;

            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool WordTableModel::existsConflictingWords(QString word, QModelIndex index)
{
    CrosswordEntry3D currentEntry = m_UserCrosswordEntries.at(index.row());

    bool conflict = false;
    for(unsigned int i = 0; i < word.size(); i++)
    {
        if(currentEntry.getGuess().at(i) == word.at(i) || currentEntry.getGuess().at(i) == QChar(46) || word.at(i) == QChar(46))
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
    for(int i = 0; i < m_RefCrosswordEntries->size(); i++)
    {
        m_UserCrosswordEntries.push_back(m_RefCrosswordEntries->at(i));
    }

    beginResetModel();
    endResetModel();
}

void WordTableModel::guessEntered(QString word, QModelIndex index)
{
    if(existsConflictingWords(word, index))
    {
        emit conflictingWordError();
        return;
    }

    m_UserCrosswordEntries.at(index.row()).setGuess(word);

    CrosswordEntry3D currentEntry = m_UserCrosswordEntries.at(index.row());

    std::vector<uivec3> letterPositions = currentEntry.getSolution().getLetterPositions();

    for(unsigned int i = 0; i < word.size(); i++)
    {
        m_RefWorkingGrid->setLetterAtLocation(word.at(i), letterPositions.at(i));
    }

    beginResetModel();
    endResetModel();
}
