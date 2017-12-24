/*! \brief The states that a crossword puzzle file may be in.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

#include <QString>

namespace CrosswordStatus
{
    typedef QString CROSSWORD_TYPE;

    const CROSSWORD_TYPE WITHOUT_ANSWERS = "Without Answers";

    const CROSSWORD_TYPE WITH_ANSWERS = "With Answers";
    const CROSSWORD_TYPE WITH_ANSWERS_FILLED_IN = "With Answers Completed";
}
