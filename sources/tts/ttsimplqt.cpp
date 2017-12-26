#include "tts/ttsimplqt.h"

TTSImplQt instance;

TTSImplQt::TTSImplQt() : m_speech{new QTextToSpeech()}
{
    setMode(SPEECH_MODES::normalSpeech);
}

TTSImplQt::~TTSImplQt()
{
    delete m_speech;
}

bool TTSImplQt::speak(QString text)
{
    preprocessText(text);

    m_speech->say(text);

    getSpeechLog().append(text);

    return true;
}

QString TTSImplQt::increaseSpeechRate()
{
    m_speech->setRate(m_speech->rate() + 0.1);
    return QString("Speech rate increased.");
}

QString TTSImplQt::decreaseSpeechRate()
{
    m_speech->setRate(m_speech->rate() - 0.1);
    return QString("Speech rate decreased.");
}

void TTSImplQt::preprocessText(QString& text)
{
    //! If the voice is in the mode where it spells letters out, split each character.
    //! Sequences of periods are also replaced with the phrase "x dots".
    if (getMode() == SPEECH_MODES::spellingOutSpeech) {
        QRegExp regexp("(\\.+)");
        regexp.setMinimal(false);

        while (text.contains(regexp)) {
            if (regexp.matchedLength() == 1) {
                text.replace(regexp.cap(0),
                             QString("</spell> ").
                             append(" dot, ").
                             append("<spell>"));
            } else {
                text.replace(regexp.cap(0),
                             QString("</spell> ").
                             append(QString::number(regexp.matchedLength())).append(" dots, ").
                             append("<spell>"));
            }
        }

        text.prepend("<spell>");
        text.append("</spell>");
    }
}
