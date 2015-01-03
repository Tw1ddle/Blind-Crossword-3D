#ifndef TTSSTUB_H
#define TTSSTUB_H

#include "ttsbase.h"

class TTSStub : public TTSBase
{
public:
    TTSStub();
    ~TTSStub();

    virtual SPEECH_MODES::SPEECHMODE getMode() const { return SPEECH_MODES::normalSpeech; }
    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode) { return true; }

    virtual QString increaseSpeechRate() { return ""; }
    virtual QString decreaseSpeechRate() { return ""; }

    virtual bool speak(QString text, DWORD options) { return true; }
};

#endif // TTSSTUB_H
