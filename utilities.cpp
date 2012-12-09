#include "utilities.h"

#include <QFile>
#include <QTextStream>

namespace Utilities
{
    bool readFile(QStringList& linelist, QString path)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return false;
        }

        QTextStream in(&file);
        QString currentLine;

        if(in.atEnd())
        {
            return false;
        }

        do
        {
            currentLine = in.readLine();
            if(currentLine.length() != 0) //skips empty lines
            {
                linelist << currentLine;
            }
        } while (!currentLine.isNull());

        return true;
    }

    bool existsFile(QString path)
    {
        QFile file(path);

        if(!file.exists())
        {
            return false;
        }

        return true;
    }

    bool writeToFile(QStringList& linelist, QFile& file)
    {
        QTextStream out(&file);

        while(!linelist.isEmpty())
        {
            out << linelist.takeFirst();
            out << "\n";
        }

        if(out.status() == QTextStream::Ok)
        {
            return true;
        }

        return false;
    }
}
