#ifdef __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_MAC

#include "ttsimplmac.h"

#import <Foundation/NSString.h>

// TODO
const DWORD SPEECH_OPTIONS::csDefaultSynchronousSpeechOptions = 0;
const DWORD SPEECH_OPTIONS::csDefaultAsynchronousSpeechOptions = 0;
const DWORD SPEECH_OPTIONS::csSpeakPunctuationOption = 0;

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
    preprocessText(text);

    [m_Voice startSpeakingString:[NSString stringWithUTF8String:text.toStdString().c_str()] ];

    getSpeechLog().append(text);

    return true;
}

void TTSImplMac::preprocessText(QString &text)
{
    TTSBase::preprocessText(text);

    //! Replace sequences of periods with the phrase "x dots".
    if(getMode() == SPEECH_MODES::spellingOutSpeech)
    {
        QRegExp regexp("(\\.+)");
        regexp.setMinimal(false);

        while(text.contains(regexp))
        {
            if(regexp.matchedLength() == 1)
            {
                text.replace(regexp.cap(0), QString(" dot, "));
            }
            else
            {
                text.replace(regexp.cap(0), QString::number(regexp.matchedLength()).append(" dots, "));
            }
        }

        text.prepend("<spell>");
        text.append("</spell>");
    }
}

#endif //TARGET_OS_MAC

#endif //__APPLE__
