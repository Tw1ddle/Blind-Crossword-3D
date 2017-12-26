#include "ui/speechlogwidget.h"

#include "tts/itexttospeech.h"

namespace ui {

SpeechLogWidget::SpeechLogWidget(QObject*) :
    QTextEdit()
{
    show();
}

// TODO move this into a slot and call from the text to speech base class "speak" method for proper updates
void SpeechLogWidget::focusInEvent(QFocusEvent* e)
{
    QTextEdit::focusInEvent(e);

    clear();

    for (int i = 0 ; i < tts::ITextToSpeech::instance().getSpeechLog().size(); i++) {
        this->append(tts::ITextToSpeech::instance().getSpeechLog().at(i));
    }
}

}
