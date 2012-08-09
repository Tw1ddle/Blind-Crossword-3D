#ifndef GRID3DGRAPHICSSCENE_H
#define GRID3DGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <vector>

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry3d.h"
#include "lettergrid.h"

class Grid3DGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    Grid3DGraphicsScene(LetterGrid* letters, std::vector<CrosswordEntry3D>* entries);
    ~Grid3DGraphicsScene();

private:
    LetterGrid* m_RefGrid;
    std::vector<CrosswordEntry3D>* m_RefCrosswordEntries;

    void build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber);

public slots:
    void buildPuzzleGrid();
    void highlightSelection();
};

#endif // GRID3DGRAPHICSSCENE_H
