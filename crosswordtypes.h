#ifndef CROSSWORDTYPES_H
#define CROSSWORDTYPES_H

#include <QString>

namespace CrosswordTypes
{
    typedef QString CROSSWORD_TYPE;

    const CROSSWORD_TYPE WITHOUT_ANSWERS = "Without Answers";

    const CROSSWORD_TYPE WITH_ANSWERS_UNSTARTED = "With Answers Unstarted";
    const CROSSWORD_TYPE WITH_ANSWERS_STARTED = "With Answers Started";
}

#endif // CROSSWORDTYPES_H
