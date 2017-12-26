#include "ui/crosswordentrytableviewcontroller.h"

#include <cassert>

#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndexList>
#include <QRegExp>
#include <QShortcut>
#include <QSortFilterProxyModel>

#include "controls/controls.h"
#include "ui/crosswordentrytablemodel.h" // we (only) need to know about the names of the headers.
#include "ui/guessworddialog.h"
#include "tts/itexttospeech.h"

namespace ui {

CrosswordEntryTableViewController::CrosswordEntryTableViewController(QWidget* parent) :
    QTableView(parent)
{
    // qt 5.0beta2 -> setSectionResizeMode
    // qt 4.8 -> setResizeMode
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    horizontalHeader()->setSectionsMovable(true);

    setTabKeyNavigation(false);
}

bool CrosswordEntryTableViewController::enterGuess()
{
    const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
    assert(proxy);

    QModelIndex currentSelection = proxy->mapToSource(selectionModel()->currentIndex());

    if (currentSelection.isValid()) {
        tts::ITextToSpeech::instance().speak("Enter your answer.");

        QString wordAtSelection = currentSelection.sibling(currentSelection.row(),
                                                           tableheader::wordColumnId).data().toString();

        assert(wordAtSelection.length() != 0);

        GuessWordDialog dialog;

        if (dialog.exec()) {
            QString guess = dialog.getLineEdit()->text().toUpper();

            if (validateInput(guess, wordAtSelection.length())) {
                emit(guessSubmitted(guess, currentSelection));
                return true;
            }
        }
    }

    return false;
}

bool CrosswordEntryTableViewController::amendGuess()
{
    const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
    assert(proxy);

    QModelIndex currentSelection = proxy->mapToSource(selectionModel()->currentIndex());

    if (currentSelection.isValid()) {
        QString wordAtSelection = currentSelection.sibling(currentSelection.row(),
                                                           tableheader::wordColumnId).data().toString();

        assert(wordAtSelection.length() != 0);

        emit guessAmendationRequested(wordAtSelection, currentSelection);

        return true;
    }

    return false;
}

bool CrosswordEntryTableViewController::eraseGuess()
{
    const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
    assert(proxy);

    QModelIndex currentSelection = proxy->mapToSource(selectionModel()->currentIndex());

    if (currentSelection.isValid()) {
        emit(guessErasureRequested(currentSelection));

        return true;
    }

    return false;
}

void CrosswordEntryTableViewController::keyPressEvent(QKeyEvent* event)
{
    QTableView::keyPressEvent(event);

    if (event->modifiers() & Qt::ControlModifier) {
        QChar pressedKey = static_cast<QChar>(event->key());
        QTableView::keyboardSearch(QString(pressedKey));
    } else {
        if (event->key() == controls::enterGuessKey ||
                event->key() == controls::enterGuessKeyAlternative) {
            enterGuess();
        } else if (event->key() == controls::amendGuessKey) {
            amendGuess();
        } else if (event->key() == controls::deleteGuessKey ||
                   event->key() == controls::deleteGuessKeyAlternative) {
            eraseGuess();
        }

        else if (event->key() == controls::readCurrentGuessKey) {
            readCurrentGuess();
        }

        else if (event->key() == controls::sortEntriesKey) {
            sortEntries();
        }
    }
}

void CrosswordEntryTableViewController::currentChanged(const QModelIndex& current,
                                                       const QModelIndex& previous)
{
    Q_UNUSED(previous);

    if (current.isValid()) {
        const QSortFilterProxyModel* proxy = dynamic_cast<const QSortFilterProxyModel*>(model());
        assert(proxy);

        QModelIndex currentSelection = proxy->mapToSource(current);
        QModelIndex previousSelection = proxy->mapToSource(previous);
        emit(modelIndexChanged(currentSelection, previousSelection));

        if (current.row() != previous.row()) {
            QString identifierAtSelection = QString("Day ").append(current.sibling(current.row(),
                                                                                   tableheader::identifierColumnId).data().toString());
            QString entryNumberAtSelection = current.sibling(current.row(),
                                                             tableheader::entryColumnId).data().toString();
            QString wordAtSelection = current.sibling(current.row(),
                                                      tableheader::wordColumnId).data().toString();
            QString clueAtSelection = current.sibling(current.row(),
                                                      tableheader::clueColumnId).data().toString();
            QString wordLengthsAtSelection = current.sibling(current.row(),
                                                             tableheader::wordLengthColumnId).data().toString().append(" letters");

            assert(!identifierAtSelection.isNull());
            assert(!entryNumberAtSelection.isNull());
            assert(!wordAtSelection.isNull());
            assert(!clueAtSelection.isNull());
            assert(!wordLengthsAtSelection.isNull());

            QString line = identifierAtSelection.
                           append(". ").
                           append(clueAtSelection).
                           append(". ").
                           append(wordLengthsAtSelection).
                           append(". ").
                           append(entryNumberAtSelection);

            tts::ITextToSpeech::instance().speak(line);
        } else if (current.column() != previous.column()) {
            if (current.column() == tableheader::identifierColumnId) {
                readCurrentIdentifier();
            } else if (current.column() == tableheader::entryColumnId) {
                readCurrentEntryNumber();
            } else if (current.column() == tableheader::wordColumnId) {
                readCurrentGuess();
            } else if (current.column() == tableheader::clueColumnId) {
                readCurrentClue();
            } else if (current.column() == tableheader::wordLengthColumnId) {
                readWordLengths();
            } else {
                assert(false);
            }
        }
    }
}

int CrosswordEntryTableViewController::sizeHintForColumn(int column) const
{
    int maxWidth = 0;

    QFontMetrics fm = QFontMetrics(this->font());

    for (int i = 0; i < model()->rowCount(); i++) {
        int width = fm.width(this->model()->index(i, column).data().toString()) + 20;

        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    return maxWidth;
}

void CrosswordEntryTableViewController::keyboardSearch(const QString& search)
{
    Q_UNUSED(search);

    // QTableView::keyboardSearch(search);
}

bool CrosswordEntryTableViewController::validateInput(QString guess, unsigned int requiredLength)
{
    if (guess.length() != requiredLength) {
        tts::ITextToSpeech::instance().speak(QString("The word has to be ").append(QString::number(
                                                                                       requiredLength)).append("characters long."));
    } else if (guess.contains(QRegExp("\\s"))) {
        tts::ITextToSpeech::instance().speak("The word must not contain spaces.");
    } else if (guess.contains(QRegExp("\\d"))) {
        tts::ITextToSpeech::instance().speak("The word must not contain numbers.");
    } else if (guess.contains(QRegExp("[^a-zA-Z&\\.]"))) {
        tts::ITextToSpeech::instance().speak("The word must not contain non-word characters.");
    } else {
        return true;
    }

    return false;
}

void CrosswordEntryTableViewController::conflictingWordError()
{
    tts::ITextToSpeech::instance().speak("The word conflicts with an intersecting word.");
}

void CrosswordEntryTableViewController::reportGuessAccepted(QString guess)
{
    tts::SPEECH_MODES::SPEECHMODE mode = tts::ITextToSpeech::instance().getMode();

    tts::ITextToSpeech::instance().setMode(tts::SPEECH_MODES::spellingOutSpeech);
    tts::ITextToSpeech::instance().speak(guess);
    tts::ITextToSpeech::instance().setMode(mode);
}

void CrosswordEntryTableViewController::reportGuessAmended(QString removedLetters)
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString wordAtSelection = currentSelection.sibling(currentSelection.row(),
                                                       tableheader::wordColumnId).data().toString();

    if (removedLetters.isNull() && !wordAtSelection.contains(QChar(Qt::Key_Period))) {
        tts::ITextToSpeech::instance().speak("Your guess is correct.");
    }

    if (removedLetters.isNull()) {
        tts::ITextToSpeech::instance().speak("There are no incorrect letters in your guess.");
    } else {
        tts::ITextToSpeech::instance().speak("Incorrect letters have been removed from your guess.");
    }
}

void CrosswordEntryTableViewController::reportGuessErased()
{
    tts::ITextToSpeech::instance().speak("Your guess has been deleted.");
}

void CrosswordEntryTableViewController::reportGuessAmendationRejected()
{
    tts::ITextToSpeech::instance().speak("Guesses cannot be validated in this puzzle");
}

void CrosswordEntryTableViewController::readCurrentIdentifier()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString entryAtSelection = currentSelection.sibling(currentSelection.row(),
                                                        tableheader::identifierColumnId).data().toString();

    tts::ITextToSpeech::instance().speak(entryAtSelection.append("."));
}

void CrosswordEntryTableViewController::readCurrentEntryNumber()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString entryAtSelection = currentSelection.sibling(currentSelection.row(),
                                                        tableheader::entryColumnId).data().toString();

    tts::ITextToSpeech::instance().speak(entryAtSelection.append("."));
}

void CrosswordEntryTableViewController::readCurrentGuess()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString wordAtSelection = currentSelection.sibling(currentSelection.row(),
                                                       tableheader::wordColumnId).data().toString();

    if (wordAtSelection.contains(QRegExp("(\\.+)"))) {
        tts::SPEECH_MODES::SPEECHMODE mode = tts::ITextToSpeech::instance().getMode();

        tts::ITextToSpeech::instance().setMode(tts::SPEECH_MODES::spellingOutSpeech);
        tts::ITextToSpeech::instance().speak(wordAtSelection);
        tts::ITextToSpeech::instance().setMode(mode);
    } else {
        tts::ITextToSpeech::instance().speak(wordAtSelection);
    }
}

void CrosswordEntryTableViewController::readCurrentClue()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString clueAtSelection = currentSelection.sibling(currentSelection.row(),
                                                       tableheader::clueColumnId).data().toString();

    tts::ITextToSpeech::instance().speak(clueAtSelection.append("."));
}

void CrosswordEntryTableViewController::readWordLengths()
{
    QModelIndex currentSelection = selectionModel()->currentIndex();
    QString wordLengthsAtSelection = currentSelection.sibling(currentSelection.row(),
                                                              tableheader::wordLengthColumnId).data().toString();

    tts::SPEECH_MODES::SPEECHMODE mode = tts::ITextToSpeech::instance().getMode();

    tts::ITextToSpeech::instance().setMode(tts::SPEECH_MODES::spellingOutSpeech);
    tts::ITextToSpeech::instance().speak(wordLengthsAtSelection);

    tts::ITextToSpeech::instance().setMode(mode);
}

void CrosswordEntryTableViewController::sortEntries()
{
    const static unsigned int cs_NumSorts = 2;
    static unsigned int s_Sort = 0;

    switch (s_Sort) {
        case 0:
            sortByColumn(tableheader::identifierColumnId, Qt::AscendingOrder);
            tts::ITextToSpeech::instance().speak("Sorted clues by day.");
            break;

        case 1:
            sortByColumn(tableheader::clueColumnId, Qt::AscendingOrder);
            tts::ITextToSpeech::instance().speak("Sorted clues by alphabetical order of clues.");
            break;
    }

    s_Sort++;

    if (s_Sort >= cs_NumSorts) {
        s_Sort = 0;
    }
}

}
