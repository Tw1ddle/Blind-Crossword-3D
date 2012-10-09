#include "ttsimpl.h"
#include <atlbase.h>
#include <atlcom.h>

#include <QRegExp>

TTSImpl instance;

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

    return success;
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

SPEECH_MODES::SPEECHMODE TTSImpl::getMode()
{
    return m_Mode;
}
