#include "printer.h"

#include <QStringList>
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextEdit>

#include "utilities.h"

const QString Printer::sc_PostalAddressFileLocation = QString("/Config/postal_address.txt");

Printer::Printer()
{
}

QTextDocument* Printer::getPuzzleDocumentForPrinting(PuzzleBase& puzzle) const
{
    QDir dir;
    QString postalAddress = getPostalAddress(dir.absolutePath().append(sc_PostalAddressFileLocation));

    QString textToPrint;

    textToPrint.append(puzzle.getInformationString().append("\n\n"));

    for(unsigned int i = 0; i < puzzle.getRefCrosswordEntries().size(); i++)
    {
        QString id = puzzle.getRefCrosswordEntries().at(i).getEntryName();
        QString direction = puzzle.getRefCrosswordEntries().at(i).getDirection().getString();
        QString answer = puzzle.getRefCrosswordEntries().at(i).getGuess().getString();

        textToPrint.append(id).append(" ").append(direction).append(" --- ").append(answer).append("\n");
    }

    textToPrint.append("\n").append(postalAddress);

    QTextEdit textViewer(textToPrint);
    QTextDocument* document = textViewer.document();

    return document;
}

QString Printer::openPrintDialog(PuzzleBase& puzzle, QWidget* parentWidget)
{
    QTextDocument* document = getPuzzleDocumentForPrinting(puzzle);

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, parentWidget);
    if(printDialog.exec() == QDialog::Accepted)
    {
        document->print(&printer);

        return QString("Sending print request to printer.");
    }

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
