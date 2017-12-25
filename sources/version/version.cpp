#include "version.h"

#include <QString>

namespace {

const static QString major = "1";
const static QString minor = "2";
const static QString applicationName = "Blind Crossword 3D";

}

namespace version {

QString getApplicationName()
{
    return QString(applicationName);
}

QString getApplicationVersion()
{
    return QString().append(major).append(".").append(minor);
}

QString getApplicationVersionDescription()
{
    return getApplicationName().append(QString(" version ")).append(getApplicationVersion());
}

}
