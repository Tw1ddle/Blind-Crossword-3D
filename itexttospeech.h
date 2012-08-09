#ifndef ITEXTTOSPEECH_H
#define ITEXTTOSPEECH_H

#include "singleton.h"
#include <QString>

#ifndef _WINDEF_
    typedef unsigned long DWORD;
#endif

#ifdef _WIN32
#include <sapi.h>
    const static DWORD csDefaultSpeechOptions = SPF_ASYNC | SPF_PURGEBEFORESPEAK;
#elif
    const static DWORD csDefaultSpeechOptions = 0;
#endif

class ITextToSpeech : public Singleton<ITextToSpeech>
{
public:
    ~ITextToSpeech() {}

    virtual bool speak(QString text, DWORD options = csDefaultSpeechOptions) = 0;

protected:
    ITextToSpeech() {}
};

#endif // ITEXTTOSPEECH_H
