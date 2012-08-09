#ifndef WORDTABLEVIEWCONTROLLER_H
#define WORDTABLEVIEWCONTROLLER_H

#include <QTableView>
#include <QModelIndex>

#include <QAccessibleWidget>

class CrosswordEntry3D;

class WordTableViewController : public QTableView, public QAccessibleWidget
{
    Q_OBJECT
public:
    explicit WordTableViewController(QWidget *parent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual int sizeHintForColumn(int column) const;

 private:
    bool enterGuess();
    bool resetGuess();
    bool validateGuess(QString guess, unsigned int requiredLength);

signals:
    void guessSubmitted(QString guess, QModelIndex index);

public slots:
    void conflictingWordError();
};

#endif // WORDTABLEVIEWCONTROLLER_H
