/*! \brief Encapsulates functionality for opening emails in a mail client.
 *  Opens an email in the default mail client.
 *  Uses QDesktopServices, for usage see: http://doc.qt.digia.com/qt/qdesktopservices.html#openUrl
 */

#pragma once

#include <QString>

namespace crossword {

class CrosswordBase;

}

namespace email {

class Emailer {
  public:
    bool openFeedbackEmail();
    bool openSendResultsEmail(crossword::CrosswordBase& puzzle);
    bool openEmail(QString address, QString subject, QString content);
};

}
