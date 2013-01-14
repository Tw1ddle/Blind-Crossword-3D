#ifdef __linux__

#include "ttsimpllinux.h"

// TODO
const DWORD SPEECH_MODES::csDefaultSynchronousSpeechOptions = 0;
const DWORD SPEECH_MODES::csDefaultAsynchronousSpeechOptions = 0;
const DWORD SPEECH_MODES::csAsynchronousNoPurgeOptions = 0;
const DWORD SPEECH_MODES::csSpeakPunctuationOption = 0;

TTSImplLinux instance;

TTSImplLinux::TTSImplLinux()
{
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

const QStringList &TTSImplLinux::getSpeechHistory() const
{
}

bool TTSImplLinux::setMode(SPEECH_MODES::SPEECHMODE mode)
{
}

SPEECH_MODES::SPEECHMODE TTSImplLinux::getMode() const
{
}

#endif // __linux__
