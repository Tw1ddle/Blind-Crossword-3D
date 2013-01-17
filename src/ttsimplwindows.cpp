#ifdef _WIN32

#include "ttsimplwindows.h"

#include <QRegExp>

const DWORD SPEECH_MODES::csDefaultSynchronousSpeechOptions = SPF_PURGEBEFORESPEAK;
const DWORD SPEECH_MODES::csDefaultAsynchronousSpeechOptions = SPF_ASYNC | SPF_PURGEBEFORESPEAK;
const DWORD SPEECH_MODES::csSpeakPunctuationOption = SPF_NLP_SPEAK_PUNC;

TTSImplWindows instance;

const float TTSImplWindows::sc_SpeedRateAdjustmentStepSize = 2.0f;

TTSImplWindows::TTSImplWindows()
{
    m_Voice = NULL;

    if(FAILED(::CoInitialize(NULL)))
    {
        m_Initialised = false;
    }

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_Voice);
    if( SUCCEEDED( hr ) )
    {
        m_Initialised = true;
    }
    else
    {
        m_Initialised = false;
    }

    setMode(SPEECH_MODES::normalSpeech);
}

TTSImplWindows::~TTSImplWindows()
{
    ::CoUninitialize();
}

bool TTSImplWindows::speak(QString text, DWORD options)
{
    preprocessText(text);

    wchar_t* array = new wchar_t[text.size() + 1];
    text.toWCharArray(array);
    array[text.size()] = 0;

    bool success = m_Voice->Speak(array, options, NULL);

    delete[] array;

    m_SpeechLog.append(text);

    return success;
}

void TTSImplWindows::preprocessText(QString& text)
{
    if(m_Mode == SPEECH_MODES::spellingOutSpeech)
    {
        QRegExp regexp("(\\.+)");
        regexp.setMinimal(false);

        while(text.contains(regexp))
        {
            if(regexp.matchedLength() == 1)
            {
                text.replace(regexp.cap(0),
                             QString("</spell> ").
                             append(" dot, ").
                             append("<spell>"));
            }
            else
            {
                text.replace(regexp.cap(0),
                             QString("</spell> ").
                             append(QString::number(regexp.matchedLength())).append(" dots, ").
                             append("<spell>"));
            }
        }

        text.prepend("<spell>");
        text.append("</spell>");
    }
}

bool TTSImplWindows::setMode(QString mode)
{
    m_Mode = mode;

    return true;
}

SPEECH_MODES::SPEECHMODE TTSImplWindows::getMode() const
{
    return m_Mode;
}

QString TTSImplWindows::increaseSpeechRate()
{
    long current;
    m_Voice->GetRate(&current);

    m_Voice->SetRate(current + sc_SpeedRateAdjustmentStepSize);

    return QString("Speech rate increased.");
}

QString TTSImplWindows::decreaseSpeechRate()
{
    long current;
    m_Voice->GetRate(&current);

    m_Voice->SetRate(current - sc_SpeedRateAdjustmentStepSize);

    return QString("Speech rate decreased.");
}

#endif //_WIN32
