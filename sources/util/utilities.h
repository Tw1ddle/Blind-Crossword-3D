/*! \brief Utility functions mostly for file I/O.
 */

#pragma once

#include <QStringList>
#include <QFile>
#include <QUrl>

namespace util {
bool readFile(QStringList& linelist, QString path);

bool writeToFile(QStringList& linelist, QFile& file);

bool existsFile(QString path);

bool openUrl(QUrl url);
}
