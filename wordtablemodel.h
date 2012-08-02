#ifndef WORDTABLEMODEL_H
#define WORDTABLEMODEL_H

#include <QAbstractTableModel>
#include "crosswordentry3d.h"

class WordTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit WordTableModel(std::vector<CrosswordEntry3D>* refCrosswordEntries, QObject *parent = 0);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    std::vector<CrosswordEntry3D>* m_RefCrosswordEntries;
    std::vector<CrosswordEntry3D> m_UserCrosswordEntries;
signals:
    
public slots:
    void crosswordEntriesChanged();
};

#endif // WORDTABLEMODEL_H
