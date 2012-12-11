/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef UTILITIES_H
#define UTILITIES_H

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

#endif // UTILITIES_H
