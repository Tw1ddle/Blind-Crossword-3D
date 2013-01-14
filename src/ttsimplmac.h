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

#include "itexttospeech.h"

#include <QString>

class TTSImplMac : public ITextToSpeech
{
public:
    TTSImplMac();
    ~TTSImplMac();

    virtual QString increaseSpeechRate();
    virtual QString decreaseSpeechRate();

    virtual bool speak(QString text, DWORD options);
    virtual const QStringList& getSpeechHistory() const;

    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode);
    virtual SPEECH_MODES::SPEECHMODE getMode() const;
};

#endif //TARGET_OS_MAC

#endif //__APPLE__

#endif // TTSIMPLMAC_H
