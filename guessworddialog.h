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
    
    QLineEdit* getLineEdit();

public slots:
    void readText(const QString& text);
    void readOnDimissal();

 private:
    QLineEdit* m_WordEdit;
};

#endif // GUESSWORDDIALOG_H
