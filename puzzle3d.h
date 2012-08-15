#ifndef PUZZLE3D_H
#define PUZZLE3D_H

#include <QString>
#include <vector>

#include "crosswordentry3d.h"
#include "puzzleloader.h"
#include "lettergrid.h"

class BCrossword3D : public QObject
{
    friend class PuzzleLoader;

public:
    BCrossword3D();

    LetterGrid& getRefGrid();
    std::vector<CrosswordEntry3D>& getRefCrosswordEntries();

    void setDimensions(uivec3 dimensions);

    QString getScoreString() const;
    QString getInformationString() const;
    QString getPuzzleTitle() const;
    FileFormats::FORMAT getPuzzleFormat() const;

    unsigned int removeIncorrectEntries();



private:
    void clear();
    unsigned int scoreSolution() const;

    LetterGrid m_Grid;

    std::vector<CrosswordEntry3D> m_CrosswordEntries;
    QString m_PuzzleTitle;
    QString m_AuthorTitle;
    QString m_PuzzleType;

    bool m_CrosswordLoaded;
    FileFormats::FORMAT m_CrosswordFileFormat;
};

#endif // PUZZLE3D_H
