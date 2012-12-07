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

namespace Utilities
{
    bool readFile(QStringList& linelist, QString path);

    bool existsFile(QString path);
}

#endif // UTILITIES_H
