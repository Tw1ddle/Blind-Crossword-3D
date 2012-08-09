#ifndef TTSIMPL_H
#define TTSIMPL_H

#include "itexttospeech.h"

class TTSImpl : public ITextToSpeech
{
public:
    TTSImpl();
    ~TTSImpl();

    virtual bool speak(QString text, DWORD options);

private:
    ISpVoice* m_Voice;
    bool m_Initialised;
};

#endif // TTSIMPL_H
