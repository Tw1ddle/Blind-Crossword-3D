#include "graphicalgridscene.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPainter>

#include <QVector3D>
#include <QMatrix4x4>

#include "assert.h"

#include "graphicalgriditem.h"
#include "crosswordgrid.h"


GraphicalGridScene::GraphicalGridScene(const CrosswordBase& puzzle) :
    QGraphicsScene(), m_RefPuzzle(puzzle), m_RefGrid(puzzle.getGrid()), m_RefCrosswordEntries(puzzle.getEntries()), m_RefBackgroundImage(puzzle.getBackgroundImage())
{
}

void GraphicalGridScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->drawPixmap(rect, m_RefBackgroundImage, QRect(0.0f, 0.0f, m_RefBackgroundImage.width(), m_RefBackgroundImage.height()));
}

void GraphicalGridScene::build2DDisc(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int discNumber)
{
    QGraphicsItemGroup* disc = new QGraphicsItemGroup();
    disc->setFlag(QGraphicsItem::ItemIsMovable);

    for(unsigned int y = 0; y < yDim; y++)
    {
        float r = y * GraphicalGridItem::sc_Size * 2.5; // magic number is for making the spacing of the rings better

        for(unsigned int x = 0; x < xDim; x++)
        {
            uivec3 index = uivec3(x, y, discNumber);
            const Letter* letter = m_RefGrid.getLetterAt(m_RefPuzzle.toGridIndex(index));
            GraphicalGridItem* item = new GraphicalGridItem(letter, discNumber);

            QVector3D position(0, -r, 0);
            QMatrix4x4 transformation;
            transformation.rotate((360/xDim) * (static_cast<float>(x) - 0.5f), 0.0, 0.0, 1.0);
            position = position * transformation;

            item->setPos(QPointF(position.x() + offset.getX(), -position.y()));

            item->setParentItem(disc);
            m_GraphicsGridItems.push_back(item);
            addItem(item);
            disc->addToGroup(item);
        }
    }

    addItem(disc);
}

void GraphicalGridScene::build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber)
{
    QGraphicsItemGroup* grid = new QGraphicsItemGroup();
    grid->setFlag(QGraphicsItem::ItemIsMovable);

    for(unsigned int y = 0; y < yDim; y++)
    {
        for(unsigned int x = 0; x < xDim; x++)
        {
            uivec3 index = uivec3(x, y, gridNumber);
            const Letter* letter = m_RefGrid.getLetterAt(m_RefPuzzle.toGridIndex(index));
            GraphicalGridItem* item = new GraphicalGridItem(letter, gridNumber);
            item->setPos(QPointF(x * GraphicalGridItem::sc_Size + offset.getX(), (yDim - y) * GraphicalGridItem::sc_Size));

            item->setParentItem(grid);
            m_GraphicsGridItems.push_back(item);
            addItem(item);
            grid->addToGroup(item);
        }
    }

    addItem(grid);
}

void GraphicalGridScene::updateGrid()
{
    for(int i = 0; i < items().size(); i++)
    {
        assert(dynamic_cast<GraphicalGridItem*>(items().at(i)));
    }

    // set the highlights
    std::vector<std::pair<uivec3, QColor> > highlights = m_RefPuzzle.getHighlights();

    for(unsigned int j = 0; j < highlights.size(); j++)
    {
        uivec3 index = m_RefPuzzle.getHighlights().at(j).first;
        GraphicalGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index)); // only works while the only graphic items are graphic grid items
        item->setColor(m_RefPuzzle.getHighlights().at(j).second);
    }
}

void GraphicalGridScene::highlightSelection(CrosswordEntry selectedCrosswordEntry)
{
    for(unsigned int i = 0; i < m_SavedColours.size(); i++)
    {
        assert(m_SavedColours.size() == m_SelectedGridLocations.size());

        uivec3 index = m_SelectedGridLocations.at(i);
        GraphicalGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index));
        item->setColor(m_SavedColours.at(i));
    }

    m_SavedColours.clear();
    m_SelectedGridLocations = selectedCrosswordEntry.getGuess().getGridLocations();


    for(unsigned int i = 0; i < m_SelectedGridLocations.size(); i++)
    {
        uivec3 index = m_SelectedGridLocations.at(i);
        GraphicalGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index));
        m_SavedColours.push_back(item->getColor());
    }

    for(unsigned int i = 0; i < m_SelectedGridLocations.size(); i++)
    {
        uivec3 index = m_SelectedGridLocations.at(i);
        GraphicalGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index));
        item->setColor(Qt::cyan);
    }

    repaintPuzzleGrid();
}

void GraphicalGridScene::buildPuzzleGrid()
{
    clear();
    m_GraphicsGridItems.clear();

        for(unsigned int z = 0; z < m_RefGrid.getDimensions().getZ(); z++)
        {
            if(m_RefPuzzle.getFormat() == FileFormats::XWC3D || m_RefPuzzle.getFormat() == FileFormats::XWC)
            {
                build2DGrid(m_RefGrid.getDimensions().getX(), m_RefGrid.getDimensions().getY(), uivec3(z * (m_RefGrid.getDimensions().getX() + 1) * GraphicalGridItem::sc_Size, 0, 0), z);
            }
            else if(m_RefPuzzle.getFormat() == FileFormats::XWCR3D)
            {
                build2DDisc(m_RefGrid.getDimensions().getX(), m_RefGrid.getDimensions().getY(), uivec3(z * (m_RefGrid.getDimensions().getX() + 1) * GraphicalGridItem::sc_Size, 0, 0), z);
            }
        }

        for(unsigned int j = 0; j < m_RefPuzzle.getHighlights().size(); j++)
        {
            uivec3 index = m_RefPuzzle.getHighlights().at(j).first;
            GraphicalGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index));
            item->setColor(m_RefPuzzle.getHighlights().at(j).second);
        }

        // set the numbers on the grid
        for(unsigned int k = 0; k < m_RefCrosswordEntries.size(); k++)
        {
            std::vector<std::pair<unsigned int, uivec3> > entryPairs = m_RefCrosswordEntries.at(k).getWordEntryStartingPositionPairs();

            for(unsigned int l = 0; l < entryPairs.size(); l++)
            {
                GraphicalGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(entryPairs.at(l).second));
                item->setCrosswordEntryNumber(entryPairs.at(l).first);
            }
        }

    setSceneRect(itemsBoundingRect());

    m_SavedColours.clear();
    m_SelectedGridLocations.clear();

    repaintPuzzleGrid();
}

void GraphicalGridScene::repaintPuzzleGrid()
{
    update();
}
