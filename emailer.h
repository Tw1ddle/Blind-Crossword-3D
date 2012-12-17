/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef EMAILER_H
#define EMAILER_H

#include <QString>

class CrosswordBase;

class Emailer
{
public:
    bool openFeedbackEmail();
    bool openSendResultsEmail(CrosswordBase& puzzle);

private:
    const static QString sc_AnswersEmailAddressFileLocation;
    const static QString sc_FeedbackEmailAddressFileLocation;

    QString getEmailAddress(const QString path) const;
};

#endif // EMAILER_H
