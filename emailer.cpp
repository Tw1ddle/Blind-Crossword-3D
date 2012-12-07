#include "emailer.h"

#include <QString>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>

#include "utilities.h"
#include "puzzlebase.h"

#include "version.h"

const QString Emailer::sc_FeedbackEmailAddressFileLocation = QString("/Config/feedback_email_address.txt");
const QString Emailer::sc_AnswersEmailAddressFileLocation = QString("/Config/answers_email_address.txt");

bool Emailer::openFeedbackEmail()
{
    QDir dir;
    QString emailAddress = getEmailAddress(dir.absolutePath().append(sc_FeedbackEmailAddressFileLocation));

    QString emailSubject = Version::getApplicationVersionDescription().append(" feedback message");

    QString emailBody;

    emailBody.append(QString("This feedback email was sent from ").append(Version::getApplicationVersionDescription()));

    QUrl mailtoURL = QUrl(QString("mailto:").append(emailAddress)
                          .append("?subject=").append(emailSubject)
                          .append("&body=").append(emailBody));

    if(QDesktopServices::openUrl(mailtoURL))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Emailer::openSendResultsEmail(PuzzleBase& puzzle)
{
    QDir dir;
    QString emailAddress = getEmailAddress(dir.absolutePath().append(sc_AnswersEmailAddressFileLocation));

    QString emailSubject = puzzle.getPuzzleTitle().append(" answers");

    QString emailBody;
    emailBody.append(puzzle.getInformationString().append("%0A%0A"));

    for(unsigned int i = 0; i < puzzle.getRefCrosswordEntries().size(); i++)
    {
        QString id = puzzle.getRefCrosswordEntries().at(i).getEntryName();
        QString direction = puzzle.getRefCrosswordEntries().at(i).getDirection().getString();
        QString answer = puzzle.getRefCrosswordEntries().at(i).getGuess().getString();

        emailBody.append(id).append(" ").append(direction).append(" --- ").append(answer).append("%0A");
    }

    QUrl mailtoURL = QUrl(QString("mailto:").append(emailAddress)
                          .append("?subject=").append(emailSubject)
                          .append("&body=").append(emailBody));

    if(QDesktopServices::openUrl(mailtoURL))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString Emailer::getEmailAddress(const QString path) const
{
    QStringList emailAddresses;
    QString emailAddress;

    Utilities::readFile(emailAddresses, path);

    if(!emailAddresses.isEmpty())
    {
        emailAddress = emailAddresses.takeFirst();
    }
    else
    {
        emailAddress = "enter@your.email_address_here.com";
    }

    return emailAddress;
}
