/*! \brief Encapsulates functionality for opening emails in a mail client.
 *  Opens an email in the default mail client.
 *  Uses QDesktopServices, for usage see: http://doc.qt.digia.com/qt/qdesktopservices.html#openUrl
 */

#pragma once

#include <QString>

class CrosswordBase;

class Emailer
{
public:
    bool openFeedbackEmail();
    bool openSendResultsEmail(CrosswordBase& puzzle);
    bool openEmail(QString address, QString subject, QString content);

private:
    const static QString sc_AnswersEmailAddressFileLocation;
    const static QString sc_FeedbackEmailAddressFileLocation;

    QString getEmailAddress(const QString path) const;
};
