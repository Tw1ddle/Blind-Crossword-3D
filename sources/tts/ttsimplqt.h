/*! \brief A cross-platform implementation of the text to speech interface.
 *  The mode setting method changes whether the speech should be spoken word by word, or letter by letter.
 */

#pragma once

#include "ttsbase.h"

#include <QTextToSpeech>

class TTSImplQt : public TTSBase {
  public:
    TTSImplQt();
    ~TTSImplQt();

    virtual QString increaseSpeechRate();
    virtual QString decreaseSpeechRate();
    virtual bool speak(QString text);

  protected:
    virtual void preprocessText(QString& text);

  private:
    QTextToSpeech* m_speech;
};
