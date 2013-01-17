#include "ttsbase.h"

TTSBase::TTSBase()
{
}

const QStringList& TTSBase::getSpeechLog() const
{
    return m_SpeechLog;
}
