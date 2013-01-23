#ifdef __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_MAC

#include "ttsimplmac.h"

#import <Foundation/NSString.h>

// TODO
const DWORD SPEECH_OPTIONS::csDefaultSynchronousSpeechOptions = 0;
const DWORD SPEECH_OPTIONS::csDefaultAsynchronousSpeechOptions = 1;
const DWORD SPEECH_OPTIONS::csSpeakPunctuationOption = 2;

TTSImplMac instance;

const float TTSImplMac::sc_SpeedRateAdjustmentStepSize = 14.0f; //~approximately measured in words per minute

TTSImplMac::TTSImplMac()
{
    m_Voice = [[NSSpeechSynthesizer alloc] initWithVoice:[NSSpeechSynthesizer defaultVoice]];

    setMode(SPEECH_MODES::normalSpeech);
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
    // If nothing is said, then stop speaking and finish, since empty string is used to stop speech on Windows
    if(text == "")
    {
        [m_Voice stopSpeaking];

        return true;
    }

    preprocessText(text);

    [m_Voice startSpeakingString:[NSString stringWithUTF8String:text.toStdString().c_str()] ];

    if(options == SPEECH_OPTIONS::csDefaultSynchronousSpeechOptions)
    {
        // Wait until the speech synth is done talking
        while([m_Voice isSpeaking])
        {
            usleep(10000); // sleep for microseconds
        }
    }

    getSpeechLog().append(text);

    return true;
}

bool TTSImplMac::setMode(SPEECH_MODES::SPEECHMODE mode)
{
    TTSBase::setMode(mode);

    //! Set the mode of the native speech engine
    if(mode == SPEECH_MODES::normalSpeech)
    {
        [m_Voice setObject:NSSpeechModeNormal forProperty:NSSpeechCharacterModeProperty error:nil];
    }
    else if(mode == SPEECH_MODES::spellingOutSpeech)
    {
        [m_Voice setObject:NSSpeechModeLiteral forProperty:NSSpeechCharacterModeProperty error:nil];
    }

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
    }
}

#endif //TARGET_OS_MAC

#endif //__APPLE__
