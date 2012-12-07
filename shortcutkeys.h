/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
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
    extern const QChar scoreShortcutKey;
    extern const QChar filePropertiesShortcutKey;
    extern const QChar filterTableViewShortcutKey;
    extern const QChar cycleSpeechModeShortcutKey;
    extern const QChar readCrosswordThemePhraseKey;
    extern const QChar stopSpeechKey;
    extern const QChar toggleApplicationOpenReminderKey;

    extern const QChar advanceClueWordKey;
    extern const QChar readCurrentClueWordKey;

    extern const QChar increaseSpeechRateKey;
    extern const QChar decreaseSpeechRateKey;

    extern const QChar enterGuessKey;
    extern const QChar amendGuessKey;
    extern const QChar deleteGuessKey;
    extern const QChar readCurrentIdentifierKey;
    extern const QChar readCurrentGuessKey;
    extern const QChar readCurrentEntryNumberKey;
    extern const QChar readCurrentClueKey;
    extern const QChar readCurrentWordLengthsKey;

    extern const QChar sortByIdentifierKey;
    extern const QChar sortByEntryKey;
    extern const QChar sortByClueKey;
    extern const QChar sortByWordLengthKey;
}

#endif // SHORTCUTKEYS_H
