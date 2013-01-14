#ifdef __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_MAC

#include "ttsimplmac.h"

// TODO
const DWORD SPEECH_MODES::csDefaultSynchronousSpeechOptions = 0;
const DWORD SPEECH_MODES::csDefaultAsynchronousSpeechOptions = 0;
const DWORD SPEECH_MODES::csAsynchronousNoPurgeOptions = 0;
const DWORD SPEECH_MODES::csSpeakPunctuationOption = 0;

TTSImplMac instance;

TTSImplMac::TTSImplMac()
{
}

TTSImplMac::~TTSImplMac()
{
}

QString TTSImplMac::increaseSpeechRate()
{
    return QString("Speech rate increased.");
}

QString TTSImplMac::decreaseSpeechRate()
{
    return QString("Speech rate decreased.");
}

bool TTSImplMac::speak(QString text, DWORD options)
{
}

const QStringList &TTSImplMac::getSpeechHistory() const
{
}

bool TTSImplMac::setMode(SPEECH_MODES::SPEECHMODE mode)
{
}

SPEECH_MODES::SPEECHMODE TTSImplMac::getMode() const
{
}

#endif //TARGET_OS_MAC

#endif //__APPLE__
