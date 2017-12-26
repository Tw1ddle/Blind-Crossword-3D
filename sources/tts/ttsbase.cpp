#include "ttsbase.h"

TTSBase::TTSBase()
{
}

QStringList &TTSBase::getSpeechLog()
{
    return m_speechLog;
}

const QStringList &TTSBase::getSpeechLog() const
{
    return m_speechLog;
}

void TTSBase::preprocessText(QString&)
{

}

SPEECH_MODES::SPEECHMODE TTSBase::getMode() const
{
    return m_mode;
}

bool TTSBase::setMode(SPEECH_MODES::SPEECHMODE mode)
{
    m_mode = mode;

    return true;
}
