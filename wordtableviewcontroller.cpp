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
    // qt 5.0beta2 -> setSectionResizeMode
    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setTabKeyNavigation(false);
}

bool WordTableViewController::enterGuess()
{
    ITextToSpeech::instance().speak("Enter your answer.");

    const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
    assert(proxy);

    QModelIndex currentSelection = proxy->mapToSource(selectionModel()->currentIndex());

    if(currentSelection.isValid())
    {
        QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 2).data().toString();

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
        QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 2).data().toString();

        assert(wordAtSelection.length() != 0);

        emit guessAmendationRequested(wordAtSelection, currentSelection);

        return true;
    }

    return false;
}

bool WordTableViewController::eraseGuess()
{
    const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
    assert(proxy);

    QModelIndex currentSelection = proxy->mapToSource(selectionModel()->currentIndex());

    if(currentSelection.isValid())
    {
        emit(guessErasureRequested(currentSelection));

        return true;
    }

    return false;
}

void WordTableViewController::keyPressEvent(QKeyEvent *event)
{
    QTableView::keyPressEvent(event);

    if(event->modifiers() & Qt::ControlModifier)
    {
        QChar pressedKey = static_cast<QChar>(event->key());
        QTableView::keyboardSearch(QString(pressedKey));
    }
    else
    {
        if(event->key() == ShortcutKeys::enterGuessKey)
        {
            enterGuess();
        }
        else if(event->key() == ShortcutKeys::amendGuessKey)
        {
            amendGuess();
        }
        else if(event->key() == ShortcutKeys::deleteGuessKey)
        {
            eraseGuess();
        }
        else if(event->key() == ShortcutKeys::readCurrentIdentifierKey)
        {
            readCurrentIdentifier();
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
}

void WordTableViewController::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if(current.isValid())
    {
        const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
        assert(proxy);

        QModelIndex currentSelection = proxy->mapToSource(current);
        QModelIndex previousSelection = proxy->mapToSource(previous);
        emit(modelIndexChanged(currentSelection, previousSelection));

        QString identifierAtSelection = current.sibling(current.row(), 0).data().toString();
        QString entryNumberAtSelection = current.sibling(current.row(), 1).data().toString();
        QString wordAtSelection = current.sibling(current.row(), 2).data().toString();
        QString clueAtSelection = current.sibling(current.row(), 3).data().toString();
        QString wordLengthsAtSelection = current.sibling(current.row(), 4).data().toString().append(" letters");

        assert(!identifierAtSelection.isNull());
        assert(!entryNumberAtSelection.isNull());
        assert(!wordAtSelection.isNull());
        assert(!clueAtSelection.isNull());
        assert(!wordLengthsAtSelection.isNull());

        QString line = identifierAtSelection.
                append(". ").
                append(entryNumberAtSelection).
                append(". ").
                append(clueAtSelection).
                append(". ").
                append(wordLengthsAtSelection);

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
   // QTableView::keyboardSearch(search);
}

bool WordTableViewController::validateInput(QString guess, unsigned int requiredLength)
{
    if(guess.length() != requiredLength)
    {
        ITextToSpeech::instance().speak(QString("The word has to be ").append(QString::number(requiredLength)).append("characters long."));
    }
    else if(guess.contains(QRegExp("\\s")))
    {
        ITextToSpeech::instance().speak("The word must not contain spaces.");
    }
    else if(guess.contains(QRegExp("\\d")))
    {
        ITextToSpeech::instance().speak("The word must not contain numbers.");
    }
    else if(guess.contains(QRegExp("[^a-zA-Z\\.]")))
    {
        ITextToSpeech::instance().speak("The word must not contain non-word characters.");
    }
    else
    {
        return true;
    }
    return false;
}

void WordTableViewController::conflictingWordError()
{
    ITextToSpeech::instance().speak("The word conflicts with an intersecting word.");
}

void WordTableViewController::reportGuessAccepted(QString guess)
{
    SPEECH_MODES::SPEECHMODE mode = ITextToSpeech::instance().getMode();

    ITextToSpeech::instance().setMode(SPEECH_MODES::spellingOutSpeech);
    ITextToSpeech::instance().speak(guess);
    ITextToSpeech::instance().setMode(mode);
}

void WordTableViewController::reportGuessAmended(QString removedLetters)
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 3).data().toString();

    if(removedLetters.isNull() && !wordAtSelection.contains(QChar(Qt::Key_Period)))
    {
        ITextToSpeech::instance().speak("Your guess is correct.");
    }
    if(removedLetters.isNull())
    {
        ITextToSpeech::instance().speak("There are no incorrect letters in your guess.");
    }
    else
    {
        ITextToSpeech::instance().speak("Incorrect letters have been removed from your guess.");
    }
}

void WordTableViewController::reportGuessErased()
{
    ITextToSpeech::instance().speak("Your guess has been deleted.");
}

void WordTableViewController::reportGuessAmendationRejected()
{
    ITextToSpeech::instance().speak("Guesses cannot be validated in this puzzle");
}

void WordTableViewController::readCurrentIdentifier()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString entryAtSelection = currentSelection.sibling(currentSelection.row(), 0).data().toString();

    ITextToSpeech::instance().speak(entryAtSelection.append("."));
}

void WordTableViewController::readCurrentEntryNumber()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString entryAtSelection = currentSelection.sibling(currentSelection.row(), 1).data().toString();

    ITextToSpeech::instance().speak(entryAtSelection.append("."));
}

void WordTableViewController::readCurrentGuess()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString wordAtSelection = currentSelection.sibling(currentSelection.row(), 2).data().toString();

    if(wordAtSelection.contains(QRegExp("(\\.+)")))
    {
        SPEECH_MODES::SPEECHMODE mode = ITextToSpeech::instance().getMode();

        ITextToSpeech::instance().setMode(SPEECH_MODES::spellingOutSpeech);
        ITextToSpeech::instance().speak(wordAtSelection);
        ITextToSpeech::instance().setMode(mode);
    }
    else
    {
        ITextToSpeech::instance().speak(wordAtSelection);
    }
}

void WordTableViewController::readCurrentClue()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString clueAtSelection = currentSelection.sibling(currentSelection.row(), 3).data().toString();

    ITextToSpeech::instance().speak(clueAtSelection.append("."));
}

void WordTableViewController::readWordLengths()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString wordLengthsAtSelection = currentSelection.sibling(currentSelection.row(), 4).data().toString();

    SPEECH_MODES::SPEECHMODE mode = ITextToSpeech::instance().getMode();

    ITextToSpeech::instance().setMode(SPEECH_MODES::spellingOutSpeech);
    ITextToSpeech::instance().speak(wordLengthsAtSelection);

    ITextToSpeech::instance().setMode(mode);
}
