/*! \brief A Mac implementation of the text to speech interface.
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef TTSIMPLMAC_H
#define TTSIMPLMAC_H

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

protected:
    virtual void preprocessText(QString& text);

private:
    NSSpeechSynthesizer* m_Voice;

    const static float sc_SpeedRateAdjustmentStepSize;
};

#endif //TARGET_OS_MAC

#endif //__APPLE__

#endif // TTSIMPLMAC_H
