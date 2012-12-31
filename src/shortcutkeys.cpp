#include "shortcutkeys.h"

#include <QKeyEvent>

namespace ShortcutKeys
{
    // management
    const QChar exitShortcutKey = Qt::Key_Q; //Quit
    const QChar loadShortcutKey = Qt::Key_L; // Load
    const QChar saveShortcutKey = Qt::Key_S; // Save
    const QChar helpShortcutKey = Qt::Key_H; // Help
    const QChar tutorialShortcutKey = Qt::Key_G; // Guide, Tutorial (T) doesn't seem to read well with SAPI voices

    const QChar confirmActionKey = Qt::Key_Y; // Yes
    const QChar rejectActionKey = Qt::Key_N; // No

    const QChar toggleApplicationOpenReminderKey = Qt::Key_D; // Disable
    const QChar decreaseSpeechRateKey = Qt::Key_8;
    const QChar increaseSpeechRateKey = Qt::Key_9;

    const QChar printAnswersKey = Qt::Key_P; // Print
    const QChar emailAnswersKey = Qt::Key_E; // Email
    const QChar emailFeedbackKey = Qt::Key_F; // Feedback

    const QChar zoomInKey = Qt::Key_Plus;
    const QChar zoomOutKey = Qt::Key_Minus;
    const QChar cycleViewVisibilityKey = Qt::Key_Apostrophe;

    // gameplay
    const QString tableNavigationKeys = "arrow keys";
    const QChar enterGuessKey = Qt::Key_Return;
    const QChar enterGuessKeyAlternative = Qt::Key_Enter; // Enter
    const QChar deleteGuessKey = Qt::Key_Backspace; // Delete
    const QChar deleteGuessKeyAlternative = Qt::Key_Delete;
    const QChar amendGuessKey = Qt::Key_V; // Validate
    const QChar markShortcutKey = Qt::Key_M; // Mark
    const QChar readCurrentGuessKey = Qt::Key_C;

    const QChar advanceClueWordKey = Qt::Key_O;
    const QChar readCurrentClueWordKey = Qt::Key_I;
    const QChar cycleSpeechModeShortcutKey = Qt::Key_J;

    const QChar filePropertiesShortcutKey = Qt::Key_A; // About
    const QChar stopSpeechKey = Qt::Key_Z;
    const QChar readLastSpokenPhraseKey = Qt::Key_R; // Repeat

    const QChar sortEntriesKey = Qt::Key_1;
    const QChar filterTableViewShortcutKey = Qt::Key_2;
}

