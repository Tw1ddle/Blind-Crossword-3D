#ifdef __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_MAC

#include "ttsimplmac.h"

#import <Foundation/NSString.h>

// TODO
const DWORD SPEECH_MODES::csDefaultSynchronousSpeechOptions = 0;
const DWORD SPEECH_MODES::csDefaultAsynchronousSpeechOptions = 0;
const DWORD SPEECH_MODES::csSpeakPunctuationOption = 0;

TTSImplMac instance;

const float TTSImplMac::sc_SpeedRateAdjustmentStepSize = 14.0f; //~approximately measured in words per minute

TTSImplMac::TTSImplMac()
{
    m_Voice = [[NSSpeechSynthesizer alloc] initWithVoice:[NSSpeechSynthesizer defaultVoice]];
}

TTSImplMac::~TTSImplMac()
{
    [m_Voice release];
}

QString TTSImplMac::increaseSpeechRate()
{
    [m_Voice setRate:[m_Voice rate] + sc_SpeedRateAdjustmentStepSize];

    return QString("Speech rate increased.");
}

QString TTSImplMac::decreaseSpeechRate()
{    
    [m_Voice setRate:[m_Voice rate] - sc_SpeedRateAdjustmentStepSize];

    return QString("Speech rate decreased.");
}

bool TTSImplMac::speak(QString text, DWORD options)
{
    [m_Voice startSpeakingString:[NSString stringWithUTF8String:text.toStdString().c_str()] ];

    return true;
}

bool TTSImplMac::setMode(SPEECH_MODES::SPEECHMODE mode)
{
    m_Mode = mode;

    return true;
}

SPEECH_MODES::SPEECHMODE TTSImplMac::getMode() const
{
    return m_Mode;
}

#endif //TARGET_OS_MAC

#endif //__APPLE__
