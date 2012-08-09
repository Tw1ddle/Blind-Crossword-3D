#include "ttsimpl.h"
#include <atlbase.h>
#include <atlcom.h>

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
}

TTSImpl::~TTSImpl()
{
    ::CoUninitialize();
}

bool TTSImpl::speak(QString text, DWORD options)
{
    wchar_t* array = new wchar_t[text.size() + 1];
    text.toWCharArray(array);
    array[text.size()] = 0;

    bool success = m_Voice->Speak(array, options, NULL);

    delete[] array;

    return success;
}
