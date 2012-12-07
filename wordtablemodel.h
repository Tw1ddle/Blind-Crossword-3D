/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef WORDTABLEMODEL_H
#define WORDTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QModelIndex>
#include <QItemSelection>

#include "puzzlebase.h"
#include "crosswordentry3d.h"
#include "lettergrid.h"

namespace WordTableHeader
{
    extern const unsigned int identifierColumnId;
    extern const unsigned int entryColumnId;
    extern const unsigned int wordColumnId;
    extern const unsigned int clueColumnId;
    extern const unsigned int wordLengthColumnId;
}

class WordTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    WordTableModel(const PuzzleBase& puzzle, std::vector<CrosswordEntry3D>& refCrosswordEntries, QObject *parent = 0);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    std::vector<CrosswordEntry3D>& m_RefCrosswordEntries;
    const LetterGrid& m_RefWorkingGrid;
    const PuzzleBase& m_RefPuzzle;

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

    void crosswordEntrySelectionChanged(CrosswordEntry3D newCrosswordEntrySelection);
};

#endif // WORDTABLEMODEL_H
