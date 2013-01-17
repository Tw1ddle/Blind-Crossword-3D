/*! \brief A singleton cross platform text to speech interface.
 *
 *  The speech output is customizable in two ways - options can be passed in with a speak method call, or modes can be set.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef ITEXTTOSPEECH_H
#define ITEXTTOSPEECH_H

#include <QString>

#include "singleton.h"

#ifndef _WINDEF_
    typedef unsigned long DWORD;
#endif

namespace SPEECH_MODES
{
    typedef QString SPEECHMODE;

    const SPEECHMODE normalSpeech = "normalSpeech";
    const SPEECHMODE spellingOutSpeech = "spellingOutSpeech";

    extern const DWORD csDefaultSynchronousSpeechOptions;
    extern const DWORD csDefaultAsynchronousSpeechOptions;
    extern const DWORD csSpeakPunctuationOption;
}

class ITextToSpeech : public Singleton<ITextToSpeech>
{
public:
    ~ITextToSpeech() {}

    virtual bool speak(QString text, DWORD options = SPEECH_MODES::csDefaultAsynchronousSpeechOptions) = 0;

    virtual QString increaseSpeechRate() = 0;
    virtual QString decreaseSpeechRate() = 0;

    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode) = 0;
    virtual SPEECH_MODES::SPEECHMODE getMode() const = 0;

    virtual const QStringList& getSpeechLog() const = 0;

protected:
    ITextToSpeech() {}
};

#endif // ITEXTTOSPEECH_H
