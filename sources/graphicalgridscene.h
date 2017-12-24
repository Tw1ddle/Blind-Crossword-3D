/*! \brief A model of the graphical grid.
 *  Responsible for maintaining a graphical model of the crossword puzzle.
 */

#pragma once

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <vector>

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry.h"
#include "crosswordbase.h"

class GridData;
class GraphicalGridItem;

class GraphicalGridScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicalGridScene(const CrosswordBase& puzzle);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    const CrosswordBase& m_RefPuzzle;
    const GridData& m_RefGrid;
    const std::vector<CrosswordEntry>& m_RefCrosswordEntries;
    const QPixmap& m_RefBackgroundImage;

    std::vector<GraphicalGridItem*> m_GraphicsGridItems;

    std::vector<uivec3> m_SelectedGridLocations;
    std::vector<QColor> m_SavedColours;

    void build2DDisc(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int discNumber);
    void build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber);

public slots:
    void buildPuzzleGrid();
    void repaintPuzzleGrid();
    void highlightSelection(CrosswordEntry selectedCrosswordEntry);
    void updateGrid();
};
