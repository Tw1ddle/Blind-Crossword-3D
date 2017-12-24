/*! \brief A Windows implementation of the text to speech interface.
 *  The Windows implementation takes options (SPEAKFLAGS, see http://msdn.microsoft.com/en-us/library/ms717252(v=vs.85).aspx) in the
 *  speak method for controlling aspects such as whether the speech should be spoken synchronously or asynchronously.
 *
 *  The mode setting method changes whether the speech should be spoken word by word, or letter by letter.
 */

#pragma once

#ifdef _WIN32

#ifndef NO_TTS

#include "ttsbase.h"

#include <atlbase.h>
#include <atlcom.h>
#include <sapi.h>

#include <QStringList>

class TTSImplWindows : public TTSBase
{
public:
    TTSImplWindows();
    ~TTSImplWindows();

    virtual QString increaseSpeechRate();
    virtual QString decreaseSpeechRate();

    virtual bool speak(QString text, DWORD options);

protected:
    virtual void preprocessText(QString& text);

private:
    ISpVoice* m_Voice;
    bool m_Initialised;

    const static float sc_SpeedRateAdjustmentStepSize;
};

#endif // NO_TTS

#endif // _WIN32
