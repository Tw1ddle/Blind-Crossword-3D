#include "graphicalgridscene.h"

#include <cassert>

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPainter>
#include <QVector3D>
#include <QMatrix4x4>

#include "dialog/graphicalgriditem.h"
#include "crossword/crosswordgrid.h"

GraphicalGridScene::GraphicalGridScene(const CrosswordBase& puzzle) :
    QGraphicsScene(), m_refPuzzle(puzzle), m_refGrid(puzzle.getGrid()),
    m_refCrosswordEntries(puzzle.getEntries()), m_refBackgroundImage(puzzle.getBackgroundImage())
{
}

void GraphicalGridScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->drawPixmap(rect, m_refBackgroundImage, QRect(0.0f, 0.0f, m_refBackgroundImage.width(),
                                                          m_refBackgroundImage.height()));
}

void GraphicalGridScene::build2DDisc(unsigned int xDim, unsigned int yDim, uivec3 offset,
                                     unsigned int discNumber)
{
    QGraphicsItemGroup* disc = new QGraphicsItemGroup();

    for (unsigned int y = 0; y < yDim; y++) {
        float r = y * GraphicalGridItem::SIZE *
                  2.5; // magic number is for making the spacing of the rings better

        for (unsigned int x = 0; x < xDim; x++) {
            uivec3 index = uivec3(x, y, discNumber);
            const Letter* letter = m_refGrid.getLetterAt(m_refPuzzle.toGridIndex(index));
            GraphicalGridItem* item = new GraphicalGridItem(letter, discNumber);

            QVector3D position(0, -r, 0);
            QMatrix4x4 transformation;
            transformation.rotate((360 / xDim) * (static_cast<float>(x) - 0.5f), 0.0, 0.0, 1.0);
            position = position * transformation;

            item->setPos(QPointF(position.x() + offset.getX(), -position.y()));

            item->setParentItem(disc);
            m_graphicsGridItems.push_back(item);
            addItem(item);
            disc->addToGroup(item);
        }
    }

    addItem(disc);
}

void GraphicalGridScene::build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset,
                                     unsigned int gridNumber)
{
    QGraphicsItemGroup* grid = new QGraphicsItemGroup();

    for (unsigned int y = 0; y < yDim; y++) {
        for (unsigned int x = 0; x < xDim; x++) {
            uivec3 index = uivec3(x, y, gridNumber);
            const Letter* letter = m_refGrid.getLetterAt(m_refPuzzle.toGridIndex(index));
            GraphicalGridItem* item = new GraphicalGridItem(letter, gridNumber);
            item->setPos(QPointF(x * GraphicalGridItem::SIZE + offset.getX(),
                                 (yDim - y) * GraphicalGridItem::SIZE));

            item->setParentItem(grid);
            m_graphicsGridItems.push_back(item);
            addItem(item);
            grid->addToGroup(item);
        }
    }

    addItem(grid);
}

void GraphicalGridScene::updateGrid()
{
    for (int i = 0; i < items().size(); i++) {
        assert(dynamic_cast<GraphicalGridItem*>(items().at(i)));
    }

    // set the highlights
    std::vector<std::pair<uivec3, QString> > highlights = m_refPuzzle.getHighlights();

    for (unsigned int j = 0; j < highlights.size(); j++) {
        uivec3 index = m_refPuzzle.getHighlights().at(j).first;
        GraphicalGridItem* item = m_graphicsGridItems.at(m_refPuzzle.toGridIndex(
                                                             index)); // only works while the only graphic items are graphic grid items
        item->setColor(m_refPuzzle.getHighlights().at(j).second);
    }
}

void GraphicalGridScene::highlightSelection(CrosswordEntry selectedCrosswordEntry)
{
    for (unsigned int i = 0; i < m_savedColours.size(); i++) {
        assert(m_savedColours.size() == m_selectedGridLocations.size());

        uivec3 index = m_selectedGridLocations.at(i);
        GraphicalGridItem* item = m_graphicsGridItems.at(m_refPuzzle.toGridIndex(index));
        item->setColor(m_savedColours.at(i));
    }

    m_savedColours.clear();
    m_selectedGridLocations = selectedCrosswordEntry.getGuess().getPositions();


    for (unsigned int i = 0; i < m_selectedGridLocations.size(); i++) {
        uivec3 index = m_selectedGridLocations.at(i);
        GraphicalGridItem* item = m_graphicsGridItems.at(m_refPuzzle.toGridIndex(index));
        m_savedColours.push_back(item->getColor());
    }

    for (unsigned int i = 0; i < m_selectedGridLocations.size(); i++) {
        uivec3 index = m_selectedGridLocations.at(i);
        GraphicalGridItem* item = m_graphicsGridItems.at(m_refPuzzle.toGridIndex(index));
        item->setColor(Qt::cyan);
    }

    repaintPuzzleGrid();
}

void GraphicalGridScene::buildPuzzleGrid()
{
    clear();
    m_graphicsGridItems.clear();

    for (unsigned int z = 0; z < m_refGrid.getDimensions().getZ(); z++) {
        if (m_refPuzzle.getFormat() == fileformat::XWC3D || m_refPuzzle.getFormat() == fileformat::XWC) {
            build2DGrid(m_refGrid.getDimensions().getX(), m_refGrid.getDimensions().getY(),
                        uivec3(z * (m_refGrid.getDimensions().getX() + 1) * GraphicalGridItem::SIZE, 0, 0), z);
        } else if (m_refPuzzle.getFormat() == fileformat::XWCR3D) {
            build2DDisc(m_refGrid.getDimensions().getX(), m_refGrid.getDimensions().getY(),
                        uivec3(z * (m_refGrid.getDimensions().getX() + 1) * GraphicalGridItem::SIZE, 0, 0), z);
        }
    }

    for (unsigned int j = 0; j < m_refPuzzle.getHighlights().size(); j++) {
        uivec3 index = m_refPuzzle.getHighlights().at(j).first;
        GraphicalGridItem* item = m_graphicsGridItems.at(m_refPuzzle.toGridIndex(index));
        item->setColor(m_refPuzzle.getHighlights().at(j).second);
    }

    // set the numbers on the grid
    for (unsigned int k = 0; k < m_refCrosswordEntries.size(); k++) {
        std::vector<std::pair<unsigned int, uivec3> > entryPairs = m_refCrosswordEntries.at(
                                                                       k).getWordEntryStartingPositionPairs();

        for (unsigned int l = 0; l < entryPairs.size(); l++) {
            GraphicalGridItem* item = m_graphicsGridItems.at(m_refPuzzle.toGridIndex(entryPairs.at(l).second));
            item->setCrosswordEntryNumber(entryPairs.at(l).first);
        }
    }

    setSceneRect(itemsBoundingRect());

    m_savedColours.clear();
    m_selectedGridLocations.clear();

    repaintPuzzleGrid();
}

void GraphicalGridScene::repaintPuzzleGrid()
{
    update();
}
