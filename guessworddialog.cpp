#include "guessworddialog.h"

#include <QLineEdit>
#include <QShortcut>

#include <QVBoxLayout>

GuessWordDialog::GuessWordDialog(QWidget *parent) :
    QDialog(parent)
{
    wordText = new QLineEdit;

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(wordText, 0, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    QShortcut* submitWord = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(submitWord, SIGNAL(activated()), this, SLOT(accept()));

    setWindowTitle(tr("Enter word"));
}
