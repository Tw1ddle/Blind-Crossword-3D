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

class PuzzleBase;

class Emailer
{
public:
    bool openFeedbackEmail();
    bool openSendResultsEmail(PuzzleBase& puzzle);

private:
    const static QString m_EmailAddressFileLocation;

    QString getEmailAddress(QString path);
};

#endif // EMAILER_H
