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

namespace CrosswordEntryTableHeader {
extern const unsigned int identifierColumnId;
extern const unsigned int entryColumnId;
extern const unsigned int wordColumnId;
extern const unsigned int clueColumnId;
extern const unsigned int wordLengthColumnId;
}

class CrosswordEntryTableModel : public QAbstractTableModel {
    Q_OBJECT
  public:
    CrosswordEntryTableModel(const CrosswordBase& puzzle,
                             std::vector<CrosswordEntry>& refCrosswordEntries, QObject* parent = 0);

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

    void crosswordEntrySelectionChanged(CrosswordEntry newCrosswordEntrySelection);

  private:
    std::vector<CrosswordEntry>& m_refEntries;
    const GridData& m_refGrid;
    const CrosswordBase& m_refPuzzle;

    bool existsConflictingWords(QString word, QModelIndex index);
};
