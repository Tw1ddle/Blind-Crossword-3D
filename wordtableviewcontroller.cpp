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
    QTableView(parent), QAccessibleWidget(parent)
{
    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

bool WordTableViewController::enterGuess()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();

    if(currentSelection.isValid())
    {
        QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 1).data().toString();

        assert(wordAtSelection.length() != 0);

        GuessWordDialog dialog;
        if(dialog.exec())
        {
            QString guess = dialog.wordText->text().toUpper();

            if(validateGuess(guess, wordAtSelection.length()))
            {
                emit(guessSubmitted(guess, currentSelection));
                return true;
            }
        }
    }

    return false;
}

bool WordTableViewController::resetGuess()
{
    return false; //TODO
}

void WordTableViewController::keyPressEvent(QKeyEvent *event)
{
    QTableView::keyPressEvent(event);

    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        enterGuess();
    }

    if(event->key() == Qt::Key_Delete)
    {
        resetGuess();
    }
}

int WordTableViewController::sizeHintForColumn(int column) const
{
    int maxWidth = 0;

    QFontMetrics fm = QFontMetrics(this->font());

    for(unsigned int i = 0; i < model()->rowCount(); i++)
    {
        int width = fm.width(this->model()->index(i, column).data().toString()) + 20;

        if(width > maxWidth)
        {
            maxWidth = width;
        }
    }

    return maxWidth;
}

bool WordTableViewController::validateGuess(QString guess, unsigned int requiredLength)
{
    if(guess.length() != requiredLength)
    {
        QMessageBox::information(this, tr("Invalid word"), tr("The word entered must be the correct length."));
    }
    else if(guess.contains(QRegExp("\\s")))
    {
        QMessageBox::information(this, tr("Invalid word"), tr("The word must not contain spaces."));
    }
    else if(guess.contains(QRegExp("\\d")))
    {
        QMessageBox::information(this, tr("Invalid word"), tr("The word must not contain numbers."));
    }
    else if(guess.contains(QRegExp("[^a-zA-Z\\.]")))
    {
        QMessageBox::information(this, tr("Invalid word"), tr("The word must not contain non-word characters."));
    }
    else
    {
        return true;
    }

    return false;
}

void WordTableViewController::conflictingWordError()
{
    QMessageBox::information(this, tr("Invalid word"), tr("The word conflicts with an intersecting word."));
}
