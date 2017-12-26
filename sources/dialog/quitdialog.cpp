#include "quitdialog.h"

#include <QShortcut>
#include <QVBoxLayout>
#include <QLabel>

#include "shortcutkeys.h"
#include "version/version.h"

QuitDialog::QuitDialog(QWidget* parent) :
    QDialog(parent)
{
    setFont(QFont("Lucida Console", 20, -1, false));

    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    m_quitTitle = QString("Quit ").append(version::getApplicationName()).append("?");
    m_quitBody =
        QString("Are you sure you want to quit the program? Unsaved progress will be lost. Press ")
        .append(ShortcutKeys::confirmActionKey).append(" to quit.")
        .append(" Press ").append(
            ShortcutKeys::rejectActionKey).append(" to return to the crossword puzzle.");

    m_acceptedText = QString("Thank you for playing").append(version::getApplicationName()).append(".");
    m_rejectedText = "Continue to solve the crossword puzzle.";

    QGridLayout* gLayout = new QGridLayout;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    QLabel* label = new QLabel(m_quitBody);
    label->setWordWrap(true);

    gLayout->addWidget(label);

    setWindowTitle(m_quitTitle);

    QShortcut* quitShortcut = new QShortcut(QKeySequence(ShortcutKeys::confirmActionKey), this);
    connect(quitShortcut, SIGNAL(activated()), this, SLOT(accept()));

    QShortcut* returnShortcut = new QShortcut(QKeySequence(ShortcutKeys::rejectActionKey), this);
    connect(returnShortcut, SIGNAL(activated()), this, SLOT(reject()));
}

QString QuitDialog::getTitleString() const
{
    return m_quitTitle;
}

QString QuitDialog::getBodyString() const
{
    return m_quitBody;
}

QString QuitDialog::getAcceptedText() const
{
    return m_acceptedText;
}

QString QuitDialog::getRejectedText() const
{
    return m_rejectedText;
}
