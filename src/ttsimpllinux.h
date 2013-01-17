/*! \brief A Linux implementation of the text to speech interface.
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef TTSIMPLLINUX_H
#define TTSIMPLLINUX_H

#ifdef __linux__

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

    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode);
    virtual SPEECH_MODES::SPEECHMODE getMode() const;
};

#endif // __linux__

#endif // TTSIMPLLINUX_H
