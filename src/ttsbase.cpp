#include "ttsbase.h"

TTSBase::TTSBase()
{
}

QStringList &TTSBase::getSpeechLog()
{
    return m_SpeechLog;
}

const QStringList &TTSBase::getSpeechLog() const
{
    return m_SpeechLog;
}

//!
//! Modify the text that is going to be spoken in useful ways
//!
void TTSBase::preprocessText(QString& text)
{

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
