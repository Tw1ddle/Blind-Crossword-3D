#include "guessworddialog.h"

#include <QLineEdit>
#include <QShortcut>
#include <QVBoxLayout>

#include "itexttospeech.h"

GuessWordDialog::GuessWordDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    m_WordEdit = new QLineEdit;

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(m_WordEdit, 0, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    QShortcut* submitWord = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(submitWord, SIGNAL(activated()), this, SLOT(accept()));

    connect(m_WordEdit, SIGNAL(textEdited(QString)), this, SLOT(readText(QString)));
    connect(this, SIGNAL(rejected()), this, SLOT(readOnDimissal()));

    setWindowTitle(tr("Enter word"));
}

QLineEdit* GuessWordDialog::getLineEdit()
{
    return m_WordEdit;
}

void GuessWordDialog::readText(const QString &text)
{
    if(text.size() >= 1)
    {
        ITextToSpeech::instance().speak(text.at(text.size() - 1));
    }
}

void GuessWordDialog::readOnDimissal()
{
    ITextToSpeech::instance().speak(tr("Text entry mode has been cancelled."));
}
