#include "dialog/guessworddialog.h"

#include <QLineEdit>
#include <QShortcut>
#include <QVBoxLayout>

#include "tts/itexttospeech.h"

GuessWordDialog::GuessWordDialog(QWidget* parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    m_wordEdit = new QLineEdit;

    QGridLayout* gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(m_wordEdit, 0, 1);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    QShortcut* submitWord = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(submitWord, SIGNAL(activated()), this, SLOT(accept()));

    connect(m_wordEdit, SIGNAL(textEdited(QString)), this, SLOT(readText(QString)));
    connect(this, SIGNAL(rejected()), this, SLOT(readOnDimissal()));

    setFont(QFont("Lucida Console", 32, -1, false));

    setWindowTitle("Enter word");
}

QLineEdit* GuessWordDialog::getLineEdit()
{
    return m_wordEdit;
}

void GuessWordDialog::readText(const QString& text)
{
    if (text.contains(Qt::Key_Space)) {
        m_wordEdit->setText(QString(text).replace(Qt::Key_Space, Qt::Key_Period));
    }

    if (text.size() >= 1) {
        tts::SPEECH_MODES::SPEECHMODE mode = tts::ITextToSpeech::instance().getMode();

        tts::ITextToSpeech::instance().setMode(tts::SPEECH_MODES::spellingOutSpeech);
        tts::ITextToSpeech::instance().speak(m_wordEdit->text().at(text.size() - 1));
        tts::ITextToSpeech::instance().setMode(mode);
    }
}

void GuessWordDialog::readOnDimissal()
{
    tts::ITextToSpeech::instance().speak("Text entry mode has been cancelled.");
}
