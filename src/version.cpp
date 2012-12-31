#include "version.h"

const QString Version::sc_Major = "1";
const QString Version::sc_Minor = "0";
const QString Version::sc_ApplicationName = "Blind Crossword 3D";

QString Version::getApplicationVersion()
{
    return QString().append(sc_Major).append(".").append(sc_Minor);

    //todo - automatic versioning
    //const static QString commits
    //const static QString git-hash
}

QString Version::getApplicationVersionDescription()
{
    return QString().append(getApplicationName()).append(QString(" version ")).append(getApplicationVersion());
}

QString Version::getApplicationName()
{
    return QString(sc_ApplicationName);
}
