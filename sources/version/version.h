/*! \brief A collection of static methods for fetching the version number and name of this program.
 */

#pragma once

#include <QString>

class Version
{
public:
    static QString getApplicationVersion();
    static QString getApplicationVersionDescription();
    static QString getApplicationName();

private:
    static const QString sc_Major;
    static const QString sc_Minor;
    static const QString sc_ApplicationName;
};
