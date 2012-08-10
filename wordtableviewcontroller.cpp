#include "wordtableviewcontroller.h"

#include <QHeaderView>
#include <QRegExp>
#include <QModelIndexList>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QShortcut>
#include <QSortFilterProxyModel>
#include <assert.h>

#include "itexttospeech.h"
#include "guessworddialog.h"

WordTableViewController::WordTableViewController(QWidget *parent) :
    QTableView(parent)
{
    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setSortingEnabled(true);
    setTabKeyNavigation(false);
}

bool WordTableViewController::enterGuess()
{
    ITextToSpeech::instance().speak(tr("Enter your answer"));

    // Risky!
    const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());

    assert(proxy);

    QModelIndex currentSelection = proxy->mapToSource(selectionModel()->currentIndex());

    if(currentSelection.isValid())
    {
        QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 1).data().toString();

        assert(wordAtSelection.length() != 0);

        GuessWordDialog dialog;
        if(dialog.exec())
        {
            QString guess = dialog.getLineEdit()->text().toUpper();

            if(validateInput(guess, wordAtSelection.length()))
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
    else if(event->key() == Qt::Key_W)
    {
        readCurrentGuess();
    }
    else if(event->key() == Qt::Key_E)
    {
        readCurrentEntryNumber();
    }
    else if(event->key() == Qt::Key_C)
    {
        readCurrentClue();
    }
    else if(event->key() == Qt::Key_B)
    {
        readWordLengths();
    }
    else if(event->key() == Qt::Key_V)
    {
        const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
        assert(proxy);
        QModelIndex currentSelection = proxy->mapToSource(selectionModel()->currentIndex());

        if(currentSelection.isValid())
        {
            emit guessAmendationRequested(currentSelection);
        }
    }
}

void WordTableViewController::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if(current.isValid())
    {
        QString entryNumberAtSelection = current.sibling(current.row(), 0).data().toString();
        QString wordAtSelection = current.sibling(current.row(), 1).data().toString();
        QString clueAtSelection = current.sibling(current.row(), 2).data().toString();

        assert(!entryNumberAtSelection.isNull());
        assert(!wordAtSelection.isNull());
        assert(!clueAtSelection.isNull());

        if(wordAtSelection.contains(QRegExp("[a-zA-Z]")))
        {
            if(wordAtSelection.contains(QRegExp("[\\.]")))
            {
                QString spelledOutWord;
                for(unsigned int i = 0; i < wordAtSelection.size(); i++)
                {
                    if(wordAtSelection.at(i) == Qt::Key_Period)
                    {
                        spelledOutWord.append(" dot. ");
                    }
                    else
                    {
                        spelledOutWord.append(QString(" ").append(wordAtSelection.at(i))).append(". ");
                    }
                }
                ITextToSpeech::instance().speak(entryNumberAtSelection.append(". ").append
                                                (spelledOutWord).append(". ").append(clueAtSelection.append(".")));
                return;
            }
            else
            {
                ITextToSpeech::instance().speak(entryNumberAtSelection.append(". ").append
                                                    (wordAtSelection).append("."));
                return;
            }
        }

        ITextToSpeech::instance().speak(entryNumberAtSelection.append(". ").append
                                        (wordAtSelection).append(". ").append(clueAtSelection.append(".")));
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

void WordTableViewController::keyboardSearch(const QString &search)
{
}

bool WordTableViewController::validateInput(QString guess, unsigned int requiredLength)
{
    if(guess.length() != requiredLength)
    {
        ITextToSpeech::instance().speak(tr("The word has to be ").append(QString::number(requiredLength)).append( tr("characters long.")));
    }
    else if(guess.contains(QRegExp("\\s")))
    {
        ITextToSpeech::instance().speak(tr("The word must not contain spaces."));
    }
    else if(guess.contains(QRegExp("\\d")))
    {
        ITextToSpeech::instance().speak(tr("The word must not contain numbers."));
    }
    else if(guess.contains(QRegExp("[^a-zA-Z\\.]")))
    {
        ITextToSpeech::instance().speak(tr("The word must not contain non-word characters."));
    }
    else
    {
        return true;
    }
    return false;
}

void WordTableViewController::conflictingWordError()
{
    ITextToSpeech::instance().speak(tr("The word conflicts with an intersecting word."));
}

void WordTableViewController::reportGuessAccepted(QString guess)
{
    ITextToSpeech::instance().speak(guess.append(tr(" entered.")));
}

void WordTableViewController::reportGuessAmended(QString removedLetters)
{
    if(removedLetters.isNull())
    {
        ITextToSpeech::instance().speak(tr("There are no incorrectly guessed letters"));
    }
    else
    {
        ITextToSpeech::instance().speak(tr("Incorrect letters have been removed from your guess"));
    }
}

void WordTableViewController::readCurrentEntryNumber()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString entryAtSelection = currentSelection.sibling(currentSelection.row(), 0).data().toString();

    ITextToSpeech::instance().speak(entryAtSelection.append("."));
}

void WordTableViewController::readCurrentGuess()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 1).data().toString();

    if(wordAtSelection.contains(QRegExp("[^\\.]")))
    {
        QString spelledOutWord;
        for(unsigned int i = 0; i < wordAtSelection.size(); i++)
        {
            if(wordAtSelection.at(i) == Qt::Key_Period)
            {
                spelledOutWord.append("dot.");
            }
            else
            {
                spelledOutWord.append(wordAtSelection.at(i)).append(".");
            }
        }
        ITextToSpeech::instance().speak(spelledOutWord);
    }
    else
    {
        ITextToSpeech::instance().speak(wordAtSelection.append("."));
    }
}

void WordTableViewController::readCurrentClue()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString clueAtSelection = currentSelection.sibling(currentSelection.row(), 2).data().toString();

    ITextToSpeech::instance().speak(clueAtSelection.append("."));
}

void WordTableViewController::readWordLengths()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString wordLengthsAtSelection = currentSelection.sibling(currentSelection.row(), 3).data().toString();

    ITextToSpeech::instance().speak(wordLengthsAtSelection.append("."));
}
