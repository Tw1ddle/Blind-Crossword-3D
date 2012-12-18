#include "ttsimpl.h"
#include <atlbase.h>
#include <atlcom.h>

#include <QRegExp>

TTSImpl instance;

const float TTSImpl::sc_SpeedRateAdjustmentStepSize = 2.0f;

TTSImpl::TTSImpl()
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

TTSImpl::~TTSImpl()
{
    ::CoUninitialize();
}

bool TTSImpl::speak(QString text, DWORD options)
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

const QStringList& TTSImpl::getSpeechHistory() const
{
    return m_SpeechLog;
}

void TTSImpl::preprocessText(QString& text)
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

bool TTSImpl::setMode(QString mode)
{
    m_Mode = mode;

    return true;
}

SPEECH_MODES::SPEECHMODE TTSImpl::getMode() const
{
    return m_Mode;
}

QString TTSImpl::increaseSpeechRate()
{
    long current;
    m_Voice->GetRate(&current);

    m_Voice->SetRate(current + sc_SpeedRateAdjustmentStepSize);

    return QString("Speech rate increased.");
}

QString TTSImpl::decreaseSpeechRate()
{
    long current;
    m_Voice->GetRate(&current);

    m_Voice->SetRate(current - sc_SpeedRateAdjustmentStepSize);

    return QString("Speech rate decreased.");
}
