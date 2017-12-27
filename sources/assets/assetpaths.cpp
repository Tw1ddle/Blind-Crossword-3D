#include "assets/assetpaths.h"

#include <QCoreApplication>
#include <QString>

#include "util/util.h"

namespace assets {

QString getFeedbackEmailAddress()
{
    return "ericwestbrook@btinternet.com";
}

QString getAnswersEmailAddress()
{
    return "sirius@calendarpuzzles.co.uk";
}

QString getPostalAddress()
{
    return "Post these answers to:\n\nCalendar Puzzles\nPO Box 4823\nCoventry\nCV6 9FN";
}

QString getWebsiteAddress()
{
    return "http://www.calendarpuzzles.co.uk/puzzle_downloads.htm";
}

QString getLicenseWebsiteAddress()
{
    return "https://raw.githubusercontent.com/Tw1ddle/Blind-Crossword-3D/master/LICENSE";
}

QString getHelpWebsiteAddress()
{
    return "http://tw1ddle.github.io/Blind-Crossword-3D/help.html";
}

QString getTutorialWebsiteAddress()
{
    return "http://tw1ddle.github.io/Blind-Crossword-3D/tutorial.html";
}

QString getEmbeddedCrosswordsFolderPath()
{
    return ":/crosswords";
}

QString getExternalCrosswordsFolderPath()
{
    return QCoreApplication::applicationDirPath() + "/crosswords";
}

}
