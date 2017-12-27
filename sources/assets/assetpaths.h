/*! \brief Locations and search paths for finding assets and configuration files that the application uses
 *  Also has utility functions for retrieving embedded config info
 */

#pragma once

#include <QString>

namespace assets {

QString getFeedbackEmailAddress();
QString getAnswersEmailAddress();
QString getPostalAddress();
QString getWebsiteAddress();
QString getLicenseWebsiteAddress();
QString getHelpWebsiteAddress();
QString getTutorialWebsiteAddress();

QString getEmbeddedBackgroundsFolderPath();
QString getEmbeddedCrosswordsFolderPath();
QString getExternalCrosswordsFolderPath();

}
