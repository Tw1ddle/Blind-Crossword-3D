#ifndef TTSIMPL_H
#define TTSIMPL_H

#include "itexttospeech.h"

class TTSImpl : public ITextToSpeech
{
public:
    TTSImpl();
    ~TTSImpl();

    virtual bool speak(QString text, DWORD options);
    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode);
    virtual SPEECH_MODES::SPEECHMODE getMode();

private:
    void preprocessText(QString& text);

    ISpVoice* m_Voice;
    bool m_Initialised;

    QString m_Mode;
};

#endif // TTSIMPL_H
