#ifndef PUZZLE3D_H
#define PUZZLE3D_H

#include <vector>
#include <QString>
#include <QMessageBox>

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry3d.h"
#include "puzzleloader.h"
#include "direction.h"
#include "lettergrid.h"

class BCrossword3D : public QObject
{
    Q_OBJECT
    friend class PuzzleLoader;

public:
    BCrossword3D();

    void setDimensions(uivec3 dimensions);

    LetterGrid& getRefGrid();
    std::vector<CrosswordEntry3D>& getRefCrosswordEntries();

    void clearPuzzle();

    unsigned int scoreSolution() const;
    unsigned int removeIncorrectEntries();

    void showInformation(QWidget* parent);

private:
    LetterGrid m_Grid;

    std::vector<CrosswordEntry3D> m_CrosswordEntries;
    QString m_PuzzleTitle;
    QString m_AuthorTitle;
    QString m_PuzzleType;

    bool m_CrosswordLoaded;
};

#endif // PUZZLE3D_H
