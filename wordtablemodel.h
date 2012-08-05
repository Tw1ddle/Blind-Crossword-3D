#ifndef WORDTABLEMODEL_H
#define WORDTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QModelIndex>
#include "crosswordentry3d.h"
#include "lettergrid.h"

extern const QString wordColumnHeader;
extern const QString clueColumnHeader;

class WordTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit WordTableModel(std::vector<CrosswordEntry3D>* refCrosswordEntries, LetterGrid* refWorkingGrid, QObject *parent = 0);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    std::vector<CrosswordEntry3D>* m_RefCrosswordEntries;
    LetterGrid* m_RefWorkingGrid;
    std::vector<CrosswordEntry3D> m_UserCrosswordEntries;

    bool existsConflictingWords(QString word, QModelIndex index);

signals:
    void conflictingWordError();

public slots:
    void crosswordEntriesChanged();
    void guessEntered(QString word, QModelIndex indes);
};

#endif // WORDTABLEMODEL_H
