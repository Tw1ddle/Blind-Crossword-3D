#include "util/util.h"

#include <QFile>
#include <QTextStream>
#include <QDesktopServices>

namespace util {

bool readFile(QStringList& linelist, QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString currentLine;

    if (in.atEnd()) {
        return false;
    }

    do {
        currentLine = in.readLine();

        if (currentLine.length() != 0) { // Skip empty lines
            linelist << currentLine;
        }
    } while (!currentLine.isNull());

    return true;
}

bool writeFile(QStringList& linelist, QFile& file)
{
    QTextStream out(&file);
    out.setCodec("UTF-8");

    while (!linelist.isEmpty()) {
        out << linelist.takeFirst();
        out << "\n";
    }

    if (out.status() == QTextStream::Ok) {
        return true;
    }

    return false;
}

bool fileExists(QString path)
{
    return QFile(path).exists();
}

bool openUrl(QUrl url)
{
    return QDesktopServices::openUrl(url);
}

}
