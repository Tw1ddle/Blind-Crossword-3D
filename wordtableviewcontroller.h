#ifndef WORDTABLEVIEWCONTROLLER_H
#define WORDTABLEVIEWCONTROLLER_H

#include <QTableView>
#include <QModelIndex>

class CrosswordEntry3D;
class QShortcut;

class WordTableViewController : public QTableView
{
    Q_OBJECT
public:
    explicit WordTableViewController(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    int sizeHintForColumn(int column) const;

    void keyboardSearch(const QString &search);

 private:
    bool enterGuess();
    bool amendGuess();
    bool validateInput(QString guess, unsigned int requiredLength);

    void readCurrentEntryNumber();
    void readCurrentGuess();
    void readCurrentClue();
    void readWordLengths();

private slots:
    void conflictingWordError();
    void reportGuessAccepted(QString guess);
    void reportGuessAmended(QString removedLetters);
    void reportGuessAmendationRejected();

signals:
    void guessSubmitted(QString guess, QModelIndex index);
    void guessAmendationRequested(QString guess, QModelIndex index);
    void modelIndexChanged(const QModelIndex& selected, const QModelIndex& deselected);
};

#endif // WORDTABLEVIEWCONTROLLER_H
