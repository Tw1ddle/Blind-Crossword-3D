/*! \brief A widget that displays a log of the text that the text to speech engine is passed. Used for debugging purposes.
 */

#pragma once

#include <QTextEdit>

namespace ui {

class SpeechLogWidget : public QTextEdit {
    Q_OBJECT
  public:
    explicit SpeechLogWidget(QObject* parent = 0);

  protected:
    void focusInEvent(QFocusEvent* e);
};

}
