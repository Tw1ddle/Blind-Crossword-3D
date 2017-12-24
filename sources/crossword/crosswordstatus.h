/*! \brief The states that a crossword puzzle file may be in.
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
