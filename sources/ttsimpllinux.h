/*! \brief A Linux implementation of the text to speech interface.
 */

#pragma once

#ifdef __linux__

#ifndef NO_TTS

#include "ttsbase.h"

#include <QString>

class TTSImplLinux : public TTSBase
{
public:
    TTSImplLinux();
    ~TTSImplLinux();

    virtual QString increaseSpeechRate();
    virtual QString decreaseSpeechRate();

    virtual bool speak(QString text, DWORD options);

protected:
    virtual void preprocessText(QString& text);
};

#endif // NO_TTS

#endif // __linux__
