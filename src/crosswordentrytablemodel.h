/*! \brief The model for the table of crossword entries
 *
 *
 *  Models the table of crossword entries, using the set of crossword entries from a crossword model as its data source.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CROSSWORDENTRYTABLEMODEL_H
#define CROSSWORDENTRYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QModelIndex>
#include <QItemSelection>

#include "crosswordbase.h"
#include "crosswordentry.h"
#include "crosswordgrid.h"

namespace CrosswordEntryTableHeader
{
    extern const unsigned int identifierColumnId;
    extern const unsigned int entryColumnId;
    extern const unsigned int wordColumnId;
    extern const unsigned int clueColumnId;
    extern const unsigned int wordLengthColumnId;
}

class CrosswordEntryTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CrosswordEntryTableModel(const CrosswordBase& puzzle, std::vector<CrosswordEntry>& refCrosswordEntries, QObject *parent = 0);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    std::vector<CrosswordEntry>& m_RefEntries;
    const GridData& m_RefGrid;
    const CrosswordBase& m_RefPuzzle;

    bool existsConflictingWords(QString word, QModelIndex index);

public slots:
    void crosswordEntriesChanged();
    void amendGuess(QString word, QModelIndex index);
    void enterGuess(QString word, QModelIndex index);
    void eraseGuess(QModelIndex index);
    void tableViewSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

signals:
    void conflictingWordError();
    void guessValidated(QString);
    void guessAmended(QString);
    void guessAmendationRequestRejected();
    void guessErased();

    void crosswordEntrySelectionChanged(CrosswordEntry newCrosswordEntrySelection);
};

#endif // CROSSWORDENTRYTABLEMODEL_H
