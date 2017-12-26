/*! \brief A singleton cross platform text to speech interface.
 *  The speech output is customizable in two ways - options can be passed in with a speak method call, or modes can be set.
 */

#pragma once

#include <QString>

#include "util/singleton.h"

namespace tts {

namespace SPEECH_MODES {
typedef QString SPEECHMODE;
const SPEECHMODE normalSpeech = "normalspeech";
const SPEECHMODE spellingOutSpeech = "spellingoutspeech";
}

class ITextToSpeech : public Singleton<ITextToSpeech> {
  public:
    ~ITextToSpeech() {}

    virtual bool speak(QString text) = 0;

    virtual QString increaseSpeechRate() = 0;
    virtual QString decreaseSpeechRate() = 0;

    virtual bool setMode(SPEECH_MODES::SPEECHMODE mode) = 0;
    virtual SPEECH_MODES::SPEECHMODE getMode() const = 0;

    virtual const QStringList& getSpeechLog() const = 0;

  protected:
    ITextToSpeech() {}
};

}
