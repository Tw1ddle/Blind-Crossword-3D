#include "wordtableviewcontroller.h"

#include <QHeaderView>
#include <QRegExp>
#include <QModelIndexList>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>

#include "assert.h"

#include "guessworddialog.h"

WordTableViewController::WordTableViewController(QWidget *parent) :
    QTableView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);

    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

bool WordTableViewController::enterGuess()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();

    if(currentSelection.isValid())
    {
        QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 0).data().toString();

        assert(wordAtSelection.length() != 0);

        GuessWordDialog dialog;
        if(dialog.exec())
        {
            QString guess = dialog.wordText->text();

            if(validateGuess(guess, wordAtSelection.length()))
            {
                emit(guessSubmitted(guess, currentSelection));
                return true;
            }
        }
    }

    return false;
}

void WordTableViewController::keyPressEvent(QKeyEvent *event)
{
    QTableView::keyPressEvent(event);

    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        enterGuess();
    }
}

bool WordTableViewController::validateGuess(QString guess, unsigned int requiredLength)
{
    if(guess.length() != requiredLength)
    {
        QMessageBox::information(this, tr("Invalid word"), tr("The word entered must be the correct length."));
        return false;
    }
    else if(guess.contains(QRegExp("\\s")))
    {
        QMessageBox::information(this, tr("Invalid word"), tr("The word must not contain spaces."));
        return false;
    }
    else if(guess.contains(QRegExp("\\d")))
    {
        QMessageBox::information(this, tr("Invalid word"), tr("The word must not contain numbers."));
        return false;
    }
    else if(guess.contains(QRegExp("[^a-zA-Z\\.]")))
    {
        QMessageBox::information(this, tr("Invalid word"), tr("The word must not contain non-word characters."));
        return false;
    }
    else
    {
        return true;
    }
}

void WordTableViewController::intersectingWordError()
{
    QMessageBox::information(this, tr("Invalid word"), tr("The word conflicts with an existing word."));
}
