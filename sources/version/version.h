/*! \brief A collection of static methods for fetching the version number and name of this program.
 */

#pragma once

#include <QString>

namespace version {

QString getApplicationName();

//!
//! Gets the version number of the application
//! \warning Version numbers are set manually.
//!
QString getApplicationVersion();

QString getApplicationVersionDescription();

}
