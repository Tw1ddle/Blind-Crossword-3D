/*! \brief A dialog into which the user enters their guesses at clues.
 *  Presents a text box for users to type in their answers, and interactively reads the content in the text box back.
 */

#pragma once

#include <QDialog>

class QLabel;
class QPushButton;
class QLineEdit;

class GuessWordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GuessWordDialog(QWidget *parent = 0);
    
    QLineEdit* getLineEdit();

public slots:
    void readText(const QString& text);
    void readOnDimissal();

 private:
    QLineEdit* m_wordEdit;
};
