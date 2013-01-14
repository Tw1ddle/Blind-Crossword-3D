/*! \brief A Windows implementation of the text to speech interface.
 *
 *  The Windows implementation takes options (SPEAKFLAGS, see http://msdn.microsoft.com/en-us/library/ms717252(v=vs.85).aspx) in the
 *  speak method for controlling aspects such as whether the speech should be spoken synchronously or asynchronously.
 *
 *  The mode setting method changes whether the speech should be spoken word by word, or letter by letter.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef TTSIMPLWINDOWS_H
#define TTSIMPWINDOWS_H

#ifdef _WIN32

#include "itexttospeech.h"

#include <atlbase.h>
#include <atlcom.h>

#include <QStringList>

class TTSImplWindows : public ITextToSpeech
{
public:
    TTSImplWindows();
    ~TTSImplWindows();

    virtual QString increaseSpeechRate();
    virtual QString decreaseSpeechRate();

    virtual bool speak(QString text, DWORD options);
    virtual const QStringList& getSpeechHistory() const;

    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode);
    virtual SPEECH_MODES::SPEECHMODE getMode() const;

private:
    void preprocessText(QString& text);

    ISpVoice* m_Voice;
    bool m_Initialised;

    QString m_Mode;
    QStringList m_SpeechLog;

    const static float sc_SpeedRateAdjustmentStepSize;
};

#endif _WIN32

#endif // TTSIMPLWINDOWS_H
