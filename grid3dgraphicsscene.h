#ifndef GRID3DGRAPHICSSCENE_H
#define GRID3DGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <vector>

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry3d.h"

class LetterGrid;

class Grid3DGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    Grid3DGraphicsScene(LetterGrid* letters, std::vector<CrosswordEntry3D>* entries, QPixmap* backgroundImage);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    LetterGrid* m_RefGrid;
    std::vector<CrosswordEntry3D>* m_RefCrosswordEntries;
    QPixmap* m_RefBackgroundImage;

    unsigned int m_CurrentlySelectedCrosswordEntryNumber;
    unsigned int m_PreviousSelectedCrosswordEntryNumber;

    void build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber);

public slots:
    void buildPuzzleGrid();
    void highlightSelection(unsigned int selectedCrosswordEntryNumber);
};

#endif // GRID3DGRAPHICSSCENE_H
