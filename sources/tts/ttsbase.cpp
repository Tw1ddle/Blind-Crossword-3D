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

void TTSBase::preprocessText(QString&)
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
