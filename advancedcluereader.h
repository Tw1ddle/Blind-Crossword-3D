#ifndef ADVANCEDCLUEREADER_H
#define ADVANCEDCLUEREADER_H

#include <QString>
#include <QStringList>

class AdvancedClueReader
{
    Q_OBJECT

public:
    AdvancedClueReader() : index(0u), separator(QChar(Qt::Key_Space))
    {
    }

    QString getWord()
    {
        QStringList words = text.split(separator);

        if(!words.at(index).isNull())
        {
            return words.at(index);
        }
        else
        {
            return "No clue selected";
        }
    }

    void advance()
    {
        QStringList words = text.split(separator);

        if(words.length() >= index)
        {
            index = 0;
        }
        else
        {
            index++;
        }
    }

public slots:
    void setText(QString text)
    {
        index = 0;
        this->text = text;
    }

private:
    QString text;
    unsigned int index;
    const QChar separator;
};

#endif // ADVANCEDCLUEREADER_H
