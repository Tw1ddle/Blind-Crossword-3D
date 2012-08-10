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

extern const DWORD csSynchronousSpeechOptions;
extern const DWORD csAsynchronousSpeechOptions;

class ITextToSpeech : public Singleton<ITextToSpeech>
{
public:
    ~ITextToSpeech() {}

    virtual bool speak(QString text, DWORD options = csAsynchronousSpeechOptions) = 0;

protected:
    ITextToSpeech() {}
};

#endif // ITEXTTOSPEECH_H
