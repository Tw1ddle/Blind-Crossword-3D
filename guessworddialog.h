#ifndef GUESSWORDDIALOG_H
#define GUESSWORDDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;
class QLineEdit;

class GuessWordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GuessWordDialog(QWidget *parent = 0);
    
    QLineEdit* wordText;

private:
    QLabel* wordLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;
};

#endif // GUESSWORDDIALOG_H
