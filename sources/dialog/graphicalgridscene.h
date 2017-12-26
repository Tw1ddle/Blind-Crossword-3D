/*! \brief A model of the graphical grid.
 *  Responsible for maintaining a graphical model of the crossword puzzle.
 */

#pragma once

#include <vector>

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPixmap>

#include "crossword/crosswordbase.h"
#include "crossword/crosswordentry.h"
#include "crossword/letter.h"
#include "util/uivec3.h"

class GridData;
class GraphicalGridItem;

class GraphicalGridScene : public QGraphicsScene {
    Q_OBJECT
  public:
    GraphicalGridScene(const CrosswordBase& puzzle);

  public slots:
    void buildPuzzleGrid();
    void repaintPuzzleGrid();
    void highlightSelection(CrosswordEntry selectedCrosswordEntry);
    void updateGrid();

  protected:
    void drawBackground(QPainter* painter, const QRectF& rect);

  private:
    void build2DDisc(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int discNumber);
    void build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber);

    const CrosswordBase& m_refPuzzle;
    const GridData& m_refGrid;
    const std::vector<CrosswordEntry>& m_refCrosswordEntries;
    const QPixmap& m_refBackgroundImage;

    std::vector<GraphicalGridItem*> m_graphicsGridItems;

    std::vector<uivec3> m_selectedGridLocations;
    std::vector<QColor> m_savedColours;
};
