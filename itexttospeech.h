#ifndef ITEXTTOSPEECH_H
#define ITEXTTOSPEECH_H

#include <QString>

#include "singleton.h"

#ifndef _WINDEF_
    typedef unsigned long DWORD;
#endif

#ifdef _WIN32
#include <sapi.h>
//#elif
// Other platforms here
#endif

extern const DWORD csDefaultSynchronousSpeechOptions;
extern const DWORD csDefaultAsynchronousSpeechOptions;
extern const DWORD csAsynchronousNoPurgeOptions;

extern const DWORD csSpeakPunctuationOption;

namespace SPEECH_MODES
{
    const QString normalSpeech = "phoneticSpeech";
    const QString spellingOutSpeech = "spellingOutSpeech";
}

class ITextToSpeech : public Singleton<ITextToSpeech>
{
public:
    ~ITextToSpeech() {}

    virtual bool speak(QString text, DWORD options = csDefaultAsynchronousSpeechOptions) = 0;
    virtual bool setMode(QString mode) = 0;

protected:
    ITextToSpeech() {}
};

#endif // ITEXTTOSPEECH_H
