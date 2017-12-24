/*! \brief A singleton cross platform text to speech interface.
 *  The speech output is customizable in two ways - options can be passed in with a speak method call, or modes can be set.
 */

#pragma once

#include <QString>

#include "singleton.h"

#ifndef _WINDEF_
    typedef unsigned long DWORD;
#endif

namespace SPEECH_MODES
{
    typedef QString SPEECHMODE;

    const SPEECHMODE normalSpeech = "normalspeech";
    const SPEECHMODE spellingOutSpeech = "spellingoutspeech";
}

namespace SPEECH_OPTIONS
{
    extern const DWORD csDefaultSynchronousSpeechOptions;
    extern const DWORD csDefaultAsynchronousSpeechOptions;
    extern const DWORD csSpeakPunctuationOption;
}

class ITextToSpeech : public Singleton<ITextToSpeech>
{
public:
    ~ITextToSpeech() {}

    virtual bool speak(QString text, DWORD options = SPEECH_OPTIONS::csDefaultAsynchronousSpeechOptions) = 0;

    virtual QString increaseSpeechRate() = 0;
    virtual QString decreaseSpeechRate() = 0;

    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode) = 0;
    virtual SPEECH_MODES::SPEECHMODE getMode() const = 0;

    virtual const QStringList& getSpeechLog() const = 0;

protected:
    ITextToSpeech() {}
};
