/*! \brief All the shortcut keys used in the program.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef SHORTCUTKEYS_H
#define SHORTCUTKEYS_H

#include <QChar>

namespace ShortcutKeys
{
    extern const QChar exitShortcutKey;
    extern const QChar loadShortcutKey;
    extern const QChar saveShortcutKey;
    extern const QChar helpShortcutKey;
    extern const QChar tutorialShortcutKey;
    extern const QChar markShortcutKey;
    extern const QChar filePropertiesShortcutKey;
    extern const QChar filterTableViewShortcutKey;
    extern const QChar cycleSpeechModeShortcutKey;
    extern const QChar stopSpeechKey;
    extern const QChar toggleApplicationOpenReminderKey;


    extern const QChar advanceClueWordKey;
    extern const QChar readCurrentClueWordKey;

    extern const QChar readCurrentGuessKey;

    extern const QChar increaseSpeechRateKey;
    extern const QChar decreaseSpeechRateKey;

    extern const QChar enterGuessKey;
    extern const QChar enterGuessKeyAlternative;
    extern const QChar amendGuessKey;
    extern const QChar deleteGuessKey;
    extern const QChar deleteGuessKeyAlternative;

    extern const QChar sortEntriesKey;

    extern const QChar printAnswersKey;
    extern const QChar emailAnswersKey;
    extern const QChar emailFeedbackKey;
    extern const QChar readLastSpokenPhraseKey;

    extern const QChar confirmActionKey;
    extern const QChar rejectActionKey;

    extern const QChar zoomInKey;
    extern const QChar zoomOutKey;
    extern const QChar cycleViewVisibilityKey;

    extern const QString tableNavigationKeys;
}

#endif // SHORTCUTKEYS_H
