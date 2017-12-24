/*! \brief A Mac implementation of the text to speech interface.
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

#ifndef NO_TTS

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_MAC

#include "ttsbase.h"

#import "NSSpeechSynthesizer.h"

class TTSImplMac : public TTSBase
{
public:
    TTSImplMac();
    ~TTSImplMac();

    virtual QString increaseSpeechRate();
    virtual QString decreaseSpeechRate();

    virtual bool speak(QString text, DWORD options);

    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode);

protected:
    virtual void preprocessText(QString& text);

private:
    NSSpeechSynthesizer* m_Voice;

    const static float sc_SpeedRateAdjustmentStepSize;
};

#endif //NO_TTS

#endif //TARGET_OS_MAC

#endif //__APPLE__
