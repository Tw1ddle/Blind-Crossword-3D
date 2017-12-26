/*! \brief Utility functions mostly for file I/O.
 */

#pragma once

#include <QFile>
#include <QStringList>
#include <QUrl>

namespace util {

bool readFile(QStringList& linelist, QString path);

bool writeFile(QStringList& linelist, QFile& file);

bool fileExists(QString path);

bool openUrl(QUrl url);

}
