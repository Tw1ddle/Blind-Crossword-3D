/*! \brief The view controller for the crossword entry table.
 *
 *
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

#include <QTableView>
#include <QModelIndex>

class CrosswordEntryTableViewController : public QTableView
{
    Q_OBJECT
public:
    explicit CrosswordEntryTableViewController(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    int sizeHintForColumn(int column) const;

    void keyboardSearch(const QString &search);

 private:
    bool enterGuess();
    bool amendGuess();
    bool eraseGuess();
    bool validateInput(QString guess, unsigned int requiredLength);

    void readCurrentIdentifier();
    void readCurrentEntryNumber();
    void readCurrentGuess();
    void readCurrentClue();
    void readWordLengths();

    void sortEntries();

private slots:
    void conflictingWordError();
    void reportGuessAccepted(QString guess);
    void reportGuessAmended(QString removedLetters);
    void reportGuessErased();
    void reportGuessAmendationRejected();

signals:
    void guessSubmitted(QString guess, QModelIndex index);
    void guessAmendationRequested(QString guess, QModelIndex index);
    void guessErasureRequested(QModelIndex index);
    void modelIndexChanged(const QModelIndex& selected, const QModelIndex& deselected);
};
