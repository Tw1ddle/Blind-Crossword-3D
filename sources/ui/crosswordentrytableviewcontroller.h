/*! \brief The view controller for the crossword entry table.
 */

#pragma once

#include <QModelIndex>
#include <QTableView>

namespace ui {

class CrosswordEntryTableViewController : public QTableView {
    Q_OBJECT
  public:
    explicit CrosswordEntryTableViewController(QWidget* parent = 0);

  public slots:
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

  protected:
    void keyPressEvent(QKeyEvent* event);
    void currentChanged(const QModelIndex& current, const QModelIndex& previous);
    int sizeHintForColumn(int column) const;
    void keyboardSearch(const QString& search);

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
};

}
