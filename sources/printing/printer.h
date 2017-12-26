/*! \brief Opens a print dialog and enables a user to print crossword answers.
 *  Opens a print dialog for printing a user's answers to a crossword.
 */

#pragma once

#include <QString>

#include "crossword/crosswordbase.h"

class QTextDocument;
class QWidget;

namespace printing {

class Printer {
  public:
    Printer();

    QString openPrintDialog(crossword::CrosswordBase& puzzle, QWidget* parentWidget);

  private:
    static const QString POSTAL_ADDRESS_LOCATION;

    QString getPostalAddress(const QString path) const;
    QTextDocument* getPuzzleDocumentForPrinting(crossword::CrosswordBase& puzzle) const;
};

}
