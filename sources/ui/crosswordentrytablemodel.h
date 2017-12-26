/*! \brief The model for the table of crossword entries
 *  Models the table of crossword entries, using the set of crossword entries from a crossword model as its data source.
 */

#pragma once

#include <QAbstractTableModel>
#include <QItemSelection>
#include <QModelIndex>
#include <QString>

#include "crossword/crosswordbase.h"
#include "crossword/crosswordentry.h"
#include "crossword/crosswordgrid.h"

namespace ui {

namespace tableheader {
const unsigned int identifierColumnId = 0;
const unsigned int entryColumnId = 4;
const unsigned int wordColumnId = 1;
const unsigned int clueColumnId = 2;
const unsigned int wordLengthColumnId = 3;
}

class CrosswordEntryTableModel : public QAbstractTableModel {
    Q_OBJECT
  public:
    CrosswordEntryTableModel(const crossword::CrosswordBase& puzzle,
                             std::vector<crossword::CrosswordEntry>& refCrosswordEntries, QObject* parent = 0);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

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

    void crosswordEntrySelectionChanged(crossword::CrosswordEntry newCrosswordEntrySelection);

  private:
    std::vector<crossword::CrosswordEntry>& m_refEntries;
    const crossword::GridData& m_refGrid;
    const crossword::CrosswordBase& m_refPuzzle;

    bool existsConflictingWords(QString word, QModelIndex index);
};

}
