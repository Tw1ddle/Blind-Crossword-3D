/*! \brief A base class for text to speech interfaces.
 *
 *  Encapsulates the common functionality for all speech interfaces, such as logging.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef TTSBASE_H
#define TTSBASE_H

#include "itexttospeech.h"

#include <QStringList>

class TTSBase : public ITextToSpeech
{
public:
    const QStringList& getSpeechLog() const;

protected:
    TTSBase();
    ~TTSBase() {}

private:
    QStringList m_SpeechLog;
};

#endif // TTSBASE_H
