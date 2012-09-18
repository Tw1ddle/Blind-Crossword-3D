#ifndef TTSIMPL_H
#define TTSIMPL_H

#include "itexttospeech.h"

class TTSImpl : public ITextToSpeech
{
public:
    TTSImpl();
    ~TTSImpl();

    virtual bool speak(QString text, DWORD options);
    virtual bool setMode(QString mode);

private:
    void preprocessText(QString& text);

    ISpVoice* m_Voice;
    bool m_Initialised;

    QString m_Mode;
};

#endif // TTSIMPL_H
