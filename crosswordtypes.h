/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef CROSSWORDTYPES_H
#define CROSSWORDTYPES_H

#include <QString>

namespace CrosswordTypes
{
    typedef QString CROSSWORD_TYPE;

    const CROSSWORD_TYPE WITHOUT_ANSWERS = "Without Answers";

    const CROSSWORD_TYPE WITH_ANSWERS = "With Answers";
    const CROSSWORD_TYPE WITH_ANSWERS_COMPLETED = "With Answers Completed";
}

#endif // CROSSWORDTYPES_H
