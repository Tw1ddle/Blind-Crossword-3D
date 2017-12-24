/*! \brief A dialog that confirms whether user wants to quit the program.
 */

#pragma once

#include <QDialog>

class QuitDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QuitDialog(QWidget *parent = 0);

    QString getTitleString() const;
    QString getBodyString() const;

    QString getAcceptedText() const;
    QString getRejectedText() const;
    
private:
    QString m_QuitTitle;
    QString m_QuitBody;
    QString m_AcceptedText;
    QString m_RejectedText;
};
