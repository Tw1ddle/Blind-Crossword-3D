/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef TTSIMPL_H
#define TTSIMPL_H

#include "itexttospeech.h"

class TTSImpl : public ITextToSpeech
{
public:
    TTSImpl();
    ~TTSImpl();

    virtual QString increaseSpeechRate();
    virtual QString decreaseSpeechRate();

    virtual bool speak(QString text, DWORD options);
    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode);
    virtual SPEECH_MODES::SPEECHMODE getMode();

private:
    void preprocessText(QString& text);

    ISpVoice* m_Voice;
    bool m_Initialised;

    QString m_Mode;

    const static float sc_SpeedRateAdjustmentStepSize;
};

#endif // TTSIMPL_H
