/*! \brief A dialog that confirms whether user wants to quit the program.
 */

#pragma once

#include <QDialog>

namespace ui {

class QuitDialog : public QDialog {
    Q_OBJECT
  public:
    explicit QuitDialog(QWidget* parent = 0);

    QString getTitleString() const;
    QString getBodyString() const;
    QString getAcceptedText() const;
    QString getRejectedText() const;
    
  private:
    QString m_quitTitle;
    QString m_quitBody;
    QString m_acceptedText;
    QString m_rejectedText;
};

}
