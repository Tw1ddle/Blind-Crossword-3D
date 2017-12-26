#include "printer.h"

#include <QStringList>
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextEdit>

#include "util/utilities.h"

const QString Printer::POSTAL_ADDRESS_LOCATION = QString("/Config/postal_address.txt");

Printer::Printer()
{
}

QTextDocument* Printer::getPuzzleDocumentForPrinting(CrosswordBase& puzzle) const
{
    QDir dir;
    QString postalAddress = getPostalAddress(dir.absolutePath().append(POSTAL_ADDRESS_LOCATION));

    QString textToPrint;

    textToPrint.append(puzzle.getInformation().append("<br/><br/>"));

    //copy and sort by calendar date
    std::vector<CrosswordEntry> entries = puzzle.getEntries();
    std::sort(entries.begin(), entries.end(), SortByIdentifier());

    for(unsigned int i = 0; i < entries.size(); i++)
    {
        QString id = entries.at(i).getIdentifier();
        QString entryName = entries.at(i).getEntry();
        QString direction = entries.at(i).getDirection();
        QString answer = entries.at(i).getGuess().getString();

        // QTextEdit understands a HTML subset and \n is treated as a space, so using <br/> tag for newlines instead
        textToPrint.append(id).append(" - ").append(entryName).append(" ").append(direction).append(" --- ").append(answer).append("<br/>");
    }

    textToPrint.append("<br/>").append(postalAddress);

    QTextEdit* textViewer = new QTextEdit(textToPrint);

    QTextDocument* document = textViewer->document();

    return document;
}

QString Printer::openPrintDialog(CrosswordBase& puzzle, QWidget* parentWidget)
{
    QTextDocument* document = getPuzzleDocumentForPrinting(puzzle);

    QFont printFont = QFont("Lucida Console", 20);
    document->setDefaultFont(printFont);

    QPrinter* printer = new QPrinter(QPrinter::HighResolution);
    QPrintDialog printDialog(printer, parentWidget);
    if(printDialog.exec() == QDialog::Accepted)
    {
        document->print(printer);

        delete document;
        delete printer;

        return QString("Sending print request to printer.");
    }

    delete document;
    delete printer;

    return QString("Print request cancelled.");
}

QString Printer::getPostalAddress(const QString path) const
{
    QString postalAddress;

    QStringList postalAddressList;

    Utilities::readFile(postalAddressList, path);

    if(!postalAddressList.isEmpty())
    {
        postalAddress = postalAddressList.join(" ");
    }
    else
    {
        postalAddress = "Error. Unable to find postal address.";
    }

    return postalAddress;
}
