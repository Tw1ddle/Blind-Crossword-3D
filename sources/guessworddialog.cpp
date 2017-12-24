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

    setFont(QFont("Lucida Console", 32, -1, false));

    setWindowTitle("Enter word");
}

QLineEdit* GuessWordDialog::getLineEdit()
{
    return m_WordEdit;
}

void GuessWordDialog::readText(const QString &text)
{
    if(text.contains(Qt::Key_Space))
    {
        m_WordEdit->setText(QString(text).replace(Qt::Key_Space, Qt::Key_Period));
    }

    if(text.size() >= 1)
    {
        SPEECH_MODES::SPEECHMODE mode = ITextToSpeech::instance().getMode();

        ITextToSpeech::instance().setMode(SPEECH_MODES::spellingOutSpeech);
        ITextToSpeech::instance().speak(m_WordEdit->text().at(text.size() - 1));
        ITextToSpeech::instance().setMode(mode);
    }
}

void GuessWordDialog::readOnDimissal()
{
    ITextToSpeech::instance().speak("Text entry mode has been cancelled.");
}
