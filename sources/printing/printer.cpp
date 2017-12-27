#include "printing/printer.h"

#include <QDir>
#include <QPrintDialog>
#include <QPrinter>
#include <QStringList>
#include <QTextEdit>

#include "assets/assetpaths.h"
#include "util/util.h"

namespace printing {

Printer::Printer()
{
}

QTextDocument* Printer::getPuzzleDocumentForPrinting(crossword::CrosswordBase& puzzle) const
{
    QDir dir;
    QString postalAddress = assets::getPostalAddress();

    QString textToPrint;

    textToPrint.append(puzzle.getInformation().append("<br/><br/>"));

    //copy and sort by calendar date
    std::vector<crossword::CrosswordEntry> entries = puzzle.getEntries();
    std::sort(entries.begin(), entries.end(), crossword::SortByIdentifier());

    for (unsigned int i = 0; i < entries.size(); i++) {
        QString id = entries.at(i).getIdentifier();
        QString entryName = entries.at(i).getEntry();
        QString direction = entries.at(i).getDirection();
        QString answer = entries.at(i).getGuess().getString();

        // QTextEdit understands a HTML subset and \n is treated as a space, so using <br/> tag for newlines instead
        textToPrint.append(id).append(" - ").append(entryName).append(" ").append(
            direction).append(" --- ").append(answer).append("<br/>");
    }

    textToPrint.append("<br/>").append(postalAddress);

    QTextEdit* textViewer = new QTextEdit(textToPrint);

    QTextDocument* document = textViewer->document();

    return document;
}

QString Printer::openPrintDialog(crossword::CrosswordBase& puzzle, QWidget* parentWidget)
{
    QTextDocument* document = getPuzzleDocumentForPrinting(puzzle);

    QFont printFont = QFont("Lucida Console", 20);
    document->setDefaultFont(printFont);

    QPrinter* printer = new QPrinter(QPrinter::HighResolution);
    QPrintDialog printDialog(printer, parentWidget);

    if (printDialog.exec() == QDialog::Accepted) {
        document->print(printer);

        delete document;
        delete printer;

        return QString("Sending print request to printer.");
    }

    delete document;
    delete printer;

    return QString("Print request cancelled.");
}

}
