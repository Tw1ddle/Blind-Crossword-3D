/*! \brief A base class for text to speech interfaces.
 *  Encapsulates the common functionality for all speech interfaces, such as logging.
 */

#pragma once

#include <QString>
#include <QStringList>

#include "tts/itexttospeech.h"

namespace tts {

class TTSBase : public ITextToSpeech {
  public:
    virtual const QStringList& getSpeechLog() const;

  protected:
    TTSBase();
    ~TTSBase() {}

    virtual void preprocessText(QString& text);
    virtual QStringList& getSpeechLog();

    virtual SPEECH_MODES::SPEECHMODE getMode() const;
    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode);

  private:
    QString m_mode;
    QStringList m_speechLog;
};

}
