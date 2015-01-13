#ifdef _WIN32

#ifndef NO_TTS

#include "ttsimplwindows.h"

const DWORD SPEECH_OPTIONS::csDefaultSynchronousSpeechOptions = SPF_PURGEBEFORESPEAK;
const DWORD SPEECH_OPTIONS::csDefaultAsynchronousSpeechOptions = SPF_ASYNC | SPF_PURGEBEFORESPEAK;
const DWORD SPEECH_OPTIONS::csSpeakPunctuationOption = SPF_NLP_SPEAK_PUNC;

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

    getSpeechLog().append(text);

    return success;
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

void TTSImplWindows::preprocessText(QString& text)
{
    //! If the voice is in the mode where it spells letters out, replace sequences of periods with the phrase "x dots".
    //! Note that <spell> tags are SAPI-specific
    if(getMode() == SPEECH_MODES::spellingOutSpeech)
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

#endif

#endif //_WIN32
