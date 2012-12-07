/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef GRID3DGRAPHICSSCENE_H
#define GRID3DGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <vector>

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry3d.h"
#include "puzzlebase.h"

class LetterGrid;
class GraphicsGridItem;

class Grid3DGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    Grid3DGraphicsScene(const PuzzleBase& puzzle);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    const PuzzleBase& m_RefPuzzle;
    const LetterGrid& m_RefGrid;
    const std::vector<CrosswordEntry3D>& m_RefCrosswordEntries;
    const QPixmap& m_RefBackgroundImage;

    std::vector<GraphicsGridItem*> m_GraphicsGridItems;

    unsigned int m_CurrentlySelectedCrosswordEntryNumber;
    unsigned int m_PreviousSelectedCrosswordEntryNumber;

    void build2DDisc(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int discNumber);
    void build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber);

public slots:
    void buildPuzzleGrid();
    void repaintPuzzleGrid();
    void highlightSelection(CrosswordEntry3D selectedCrosswordEntry);
};

#endif // GRID3DGRAPHICSSCENE_H
