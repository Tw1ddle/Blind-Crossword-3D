#include "shortcutkeys.h"

#include <QKeyEvent>

namespace ShortcutKeys
{
    // gameplay
    const QChar enterGuessKey = Qt::Key_Return;
    const QChar enterGuessKeyAlternative = Qt::Key_Enter; // Enter
    const QChar amendGuessKey = Qt::Key_V; // Validate
    const QChar deleteGuessKey = Qt::Key_Backspace; // Delete
    const QChar deleteGuessKeyAlternative = Qt::Key_Delete;

    const QChar markShortcutKey = Qt::Key_K; // Mark
    const QChar filePropertiesShortcutKey = Qt::Key_P; // Properties
    const QChar filterTableViewShortcutKey = Qt::Key_F; // Filter
    const QChar cycleSpeechModeShortcutKey = Qt::Key_J; // Mode
    const QChar stopSpeechKey = Qt::Key_Z; // Silence

    const QChar readLastSpokenPhraseKey = Qt::Key_R; // Repeat

    const QChar advanceClueWordKey = Qt::Key_O;
    const QChar readCurrentClueWordKey = Qt::Key_I;

    const QChar toggleApplicationOpenReminderKey = Qt::Key_N; // Toggle
    const QChar cycleViewVisibilityKey = Qt::Key_Apostrophe;

    const QChar sortEntriesKey = Qt::Key_1;
    const QChar sortByClueKey = Qt::Key_2;

    // other
    const QChar exitShortcutKey = Qt::Key_Q; //Quit
    const QChar loadShortcutKey = Qt::Key_L; // Load
    const QChar saveShortcutKey = Qt::Key_S; // Save
    const QChar helpShortcutKey = Qt::Key_H; // Help
    const QChar tutorialShortcutKey = Qt::Key_T; // Tutorial

    const QChar confirmActionKey = Qt::Key_Y; // Yes
    const QChar rejectActionKey = Qt::Key_N; // No

    const QChar decreaseSpeechRateKey = Qt::Key_9;
    const QChar increaseSpeechRateKey = Qt::Key_0;
    const QChar printAnswersKey = Qt::Key_D;
    const QChar emailAnswersKey = Qt::Key_G;
    const QChar emailFeedbackKey = Qt::Key_B;
}

