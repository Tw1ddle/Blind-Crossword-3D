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
#include "shortcutkeys.h"

WordTableViewController::WordTableViewController(QWidget *parent) :
    QTableView(parent)
{
    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

  //  setSortingEnabled(true);
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

bool WordTableViewController::amendGuess()
{
    const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
    assert(proxy);

    QModelIndex currentSelection = proxy->mapToSource(selectionModel()->currentIndex());

    if(currentSelection.isValid())
    {
        QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 1).data().toString();

        assert(wordAtSelection.length() != 0);

        emit guessAmendationRequested(wordAtSelection, currentSelection);

        return true;
    }

    return false;
}

void WordTableViewController::keyPressEvent(QKeyEvent *event)
{
    QTableView::keyPressEvent(event);

    if(event->key() == ShortcutKeys::enterGuessKey)
    {
        enterGuess();
    }
    else if(event->key() == ShortcutKeys::amendGuessKey)
    {
        amendGuess();
    }
    else if(event->key() == ShortcutKeys::readCurrentGuessKey)
    {
        readCurrentGuess();
    }
    else if(event->key() == ShortcutKeys::readCurrentEntryNumberKey)
    {
        readCurrentEntryNumber();
    }
    else if(event->key() == ShortcutKeys::readCurrentClueKey)
    {
        readCurrentClue();
    }
    else if(event->key() == ShortcutKeys::readCurrentWordLengthsKey)
    {
        readWordLengths();
    }
}

void WordTableViewController::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if(current.isValid())
    {
        QString entryNumberAtSelection = current.sibling(current.row(), 0).data().toString();
        QString wordAtSelection = current.sibling(current.row(), 1).data().toString(); // Really weird delimiter appears, \000 at the end of this string after the assertions (at least from watching the debugger)
        QString clueAtSelection = current.sibling(current.row(), 2).data().toString();
        QString wordLengthsAtSelection = current.sibling(current.row(), 3).data().toString();

        assert(!entryNumberAtSelection.isNull());
        assert(!wordAtSelection.isNull());
        assert(!clueAtSelection.isNull());
        assert(!wordLengthsAtSelection.isNull());

        QString line = entryNumberAtSelection.append(". ").append(clueAtSelection.append(". ")).append(wordLengthsAtSelection).append(".");

        ITextToSpeech::instance().speak(line);
    }
}

int WordTableViewController::sizeHintForColumn(int column) const
{
    int maxWidth = 0;

    QFontMetrics fm = QFontMetrics(this->font());

    for(int i = 0; i < model()->rowCount(); i++)
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
    Q_UNUSED(search);
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

    if(wordAtSelection.contains(QRegExp("[\\.]")))
    {
        QString spelledOutWord = QString("<spell>").append(wordAtSelection).append("</spell>");
        ITextToSpeech::instance().speak(spelledOutWord);
    }
    else
    {
        ITextToSpeech::instance().speak(wordAtSelection);
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
