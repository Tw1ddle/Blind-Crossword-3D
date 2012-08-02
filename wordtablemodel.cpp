#include "wordtablemodel.h"

WordTableModel::WordTableModel(std::vector<CrosswordEntry3D>* refCrosswordEntries, QObject *parent) :
    QAbstractTableModel(parent)
{
    m_RefCrosswordEntries = refCrosswordEntries;

    for(int i = 0; i < m_RefCrosswordEntries->size(); i++)
    {
        m_UserCrosswordEntries.push_back(m_RefCrosswordEntries->at(i));
    }
}

int WordTableModel::rowCount(const QModelIndex& parent) const
{
    return m_RefCrosswordEntries->size();
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

    if (index.row() >= m_RefCrosswordEntries->size() || index.row() < 0)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return m_RefCrosswordEntries->at(index.row()).getGuess();
        }
        else if (index.column() == 1)
        {
            return m_RefCrosswordEntries->at(index.row()).getHint().getString();
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
                return tr("Word");

            case 1:
                return tr("Hint");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

void WordTableModel::crosswordEntriesChanged()
{
    beginResetModel();
    endResetModel();
}

