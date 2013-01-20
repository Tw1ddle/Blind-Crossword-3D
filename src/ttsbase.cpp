#include "ttsbase.h"

TTSBase::TTSBase()
{
}

const QStringList& TTSBase::getSpeechLog() const
{
    return m_SpeechLog;
}

//!
//! Modify the text that is going to be spoken in useful ways
//!
void TTSBase::preprocessText(QString& text)
{

}

QStringList &TTSBase::getSpeechLog()
{
    return m_SpeechLog;
}

SPEECH_MODES::SPEECHMODE TTSBase::getMode() const
{
    return m_Mode;
}

bool TTSBase::setMode(SPEECH_MODES::SPEECHMODE mode)
{
    m_Mode = mode;

    return true;
}
