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

namespace crossword {

class GridData;

}

namespace ui {

class GraphicalGridItem;

}

namespace ui {

class GraphicalGridScene : public QGraphicsScene {
    Q_OBJECT
  public:
    GraphicalGridScene(const crossword::CrosswordBase& puzzle);

  public slots:
    void buildPuzzleGrid();
    void repaintPuzzleGrid();
    void highlightSelection(crossword::CrosswordEntry selectedCrosswordEntry);
    void updateGrid();

  protected:
    void drawBackground(QPainter* painter, const QRectF& rect);

  private:
    void build2DDisc(unsigned int xDim, unsigned int yDim, util::uivec3 offset,
                     unsigned int discNumber);
    void build2DGrid(unsigned int xDim, unsigned int yDim, util::uivec3 offset,
                     unsigned int gridNumber);

    const crossword::CrosswordBase& m_refPuzzle;
    const crossword::GridData& m_refGrid;
    const std::vector<crossword::CrosswordEntry>& m_refCrosswordEntries;
    const QPixmap& m_refBackgroundImage;

    std::vector<GraphicalGridItem*> m_graphicsGridItems;

    std::vector<util::uivec3> m_selectedGridLocations;
    std::vector<QColor> m_savedColours;
};

}
