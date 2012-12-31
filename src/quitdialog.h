/*! \brief A dialog that confirms whether user wants to quit the program.
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef QUITDIALOG_H
#define QUITDIALOG_H

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

#endif // QUITDIALOG_H
