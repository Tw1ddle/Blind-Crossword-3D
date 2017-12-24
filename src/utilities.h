/*! \brief Utility functions mostly for file I/O.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

#include <QStringList>
#include <QFile>
#include <QUrl>

namespace Utilities
{
    bool readFile(QStringList& linelist, QString path);

    bool writeToFile(QStringList& linelist, QFile& file);

    bool existsFile(QString path);

    bool openUrl(QUrl url);
}
