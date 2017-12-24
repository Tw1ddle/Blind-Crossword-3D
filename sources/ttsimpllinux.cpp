#ifdef __linux__

#ifndef NO_TTS

#include "ttsimpllinux.h"

// TODO
const DWORD SPEECH_OPTIONS::csDefaultSynchronousSpeechOptions = 0;
const DWORD SPEECH_OPTIONS::csDefaultAsynchronousSpeechOptions = 0;
const DWORD SPEECH_OPTIONS::csSpeakPunctuationOption = 0;

TTSImplLinux instance;

TTSImplLinux::TTSImplLinux()
{
    setMode(SPEECH_MODES::normalSpeech);
}

QString TTSImplLinux::increaseSpeechRate()
{
    return QString("Speech rate increased.");
}

QString TTSImplLinux::decreaseSpeechRate()
{
    return QString("Speech rate decreased.");
}

bool TTSImplLinux::speak(QString text, DWORD options)
{
}

void TTSImplLinux::preprocessText(QString& text)
{
    TTSBase::preprocessText(text);
}

#endif

#endif // __linux__
