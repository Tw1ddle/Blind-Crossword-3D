#ifndef WORDTABLEVIEWCONTROLLER_H
#define WORDTABLEVIEWCONTROLLER_H

#include <QTableView>
#include <QModelIndex>

class CrosswordEntry3D;

class WordTableViewController : public QTableView
{
    Q_OBJECT
public:
    explicit WordTableViewController(QWidget *parent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

 private:
    bool enterGuess();
    bool validateGuess(QString guess, unsigned int requiredLength);

signals:
    void guessSubmitted(QString guess, QModelIndex index);

public slots:
    void intersectingWordError();
};

#endif // WORDTABLEVIEWCONTROLLER_H
