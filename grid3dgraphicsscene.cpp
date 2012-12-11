#include "grid3dgraphicsscene.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPainter>

#include <QVector3D>
#include <QMatrix4x4>

#include "assert.h"

#include "graphicsgriditem.h"
#include "lettergrid.h"


Grid3DGraphicsScene::Grid3DGraphicsScene(const PuzzleBase& puzzle) :
    QGraphicsScene(), m_RefPuzzle(puzzle), m_RefGrid(puzzle.getGrid()), m_RefCrosswordEntries(puzzle.getCrosswordEntries()), m_RefBackgroundImage(puzzle.getPuzzleBackgroundImage())
{
}

void Grid3DGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->drawPixmap(rect, m_RefBackgroundImage, QRect(0.0f, 0.0f, m_RefBackgroundImage.width(), m_RefBackgroundImage.height()));
}

void Grid3DGraphicsScene::build2DDisc(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int discNumber)
{
    QGraphicsItemGroup* disc = new QGraphicsItemGroup();
    disc->setFlag(QGraphicsItem::ItemIsMovable);

    for(unsigned int y = 0; y < yDim; y++)
    {
        float r = y * GraphicsGridItem::sc_Size * 2.5; // magic number is for making the spacing of the rings better

        for(unsigned int x = 0; x < xDim; x++)
        {
            uivec3 index = uivec3(x, y, discNumber);
            const Letter* letter = m_RefGrid.getLetterAt(m_RefPuzzle.toGridIndex(index));
            GraphicsGridItem* item = new GraphicsGridItem(letter, discNumber);

            QVector3D position(0, -r, 0);
            QMatrix4x4 transformation;
            transformation.rotate((360/xDim) * x, 0.0, 0.0, 1.0);
            position = position * transformation;

            item->setPos(QPointF(position.x() + offset.getX(), position.y()));

            item->setParentItem(disc);
            m_GraphicsGridItems.push_back(item);
            addItem(item);
            disc->addToGroup(item);
        }
    }

    addItem(disc);
}

void Grid3DGraphicsScene::build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber)
{
    QGraphicsItemGroup* grid = new QGraphicsItemGroup();
    grid->setFlag(QGraphicsItem::ItemIsMovable);

    for(unsigned int y = 0; y < yDim; y++)
    {
        for(unsigned int x = 0; x < xDim; x++)
        {
            uivec3 index = uivec3(x, y, gridNumber);
            const Letter* letter = m_RefGrid.getLetterAt(m_RefPuzzle.toGridIndex(index));
            GraphicsGridItem* item = new GraphicsGridItem(letter, gridNumber);
            item->setPos(QPointF(x * GraphicsGridItem::sc_Size + offset.getX(), (yDim - y) * GraphicsGridItem::sc_Size));

            item->setParentItem(grid);
            m_GraphicsGridItems.push_back(item);
            addItem(item);
            grid->addToGroup(item);
        }
    }

    addItem(grid);
}

void Grid3DGraphicsScene::updateGrid()
{
    for(unsigned int i = 0; i < items().size(); i++)
    {
        assert(dynamic_cast<GraphicsGridItem*>(items().at(i)));
    }

    // set the highlights
    std::vector<uivec3> highlights = m_RefPuzzle.getThemePhraseCoordinates();

    for(unsigned int j = 0; j < highlights.size(); j++)
    {
        uivec3 index = m_RefPuzzle.getThemePhraseCoordinates().at(j);
        GraphicsGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index)); // only works while the only graphic items are graphic grid items
        item->setColor(Qt::yellow);
    }
}

void Grid3DGraphicsScene::highlightSelection(CrosswordEntry3D selectedCrosswordEntry)
{
    for(unsigned int i = 0; i < m_SavedColours.size(); i++)
    {
        assert(m_SavedColours.size() == m_SelectedGridLocations.size());

        uivec3 index = m_SelectedGridLocations.at(i);
        GraphicsGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index));
        item->setColor(m_SavedColours.at(i));
    }

    m_SavedColours.clear();
    m_SelectedGridLocations = selectedCrosswordEntry.getGuess().getGridLocations();


    for(unsigned int i = 0; i < m_SelectedGridLocations.size(); i++)
    {
        uivec3 index = m_SelectedGridLocations.at(i);
        GraphicsGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index));
        m_SavedColours.push_back(item->getColor());
    }

    for(unsigned int i = 0; i < m_SelectedGridLocations.size(); i++)
    {
        uivec3 index = m_SelectedGridLocations.at(i);
        GraphicsGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index));
        item->setColor(Qt::cyan);
    }

    repaintPuzzleGrid();
}

void Grid3DGraphicsScene::buildPuzzleGrid()
{
    clear();
    m_GraphicsGridItems.clear();

        for(unsigned int z = 0; z < m_RefGrid.getDimensions().getZ(); z++)
        {
            if(m_RefPuzzle.getPuzzleFormat() == FileFormats::XWC3D || m_RefPuzzle.getPuzzleFormat() == FileFormats::XWC)
            {
                build2DGrid(m_RefGrid.getDimensions().getX(), m_RefGrid.getDimensions().getY(), uivec3(z * (m_RefGrid.getDimensions().getX() + 1) * GraphicsGridItem::sc_Size, 0, 0), z);
            }
            else if(m_RefPuzzle.getPuzzleFormat() == FileFormats::XWCR3D)
            {
                build2DDisc(m_RefGrid.getDimensions().getX(), m_RefGrid.getDimensions().getY(), uivec3(z * (m_RefGrid.getDimensions().getX() + 1) * GraphicsGridItem::sc_Size, 0, 0), z);
            }
        }

        for(unsigned int j = 0; j < m_RefPuzzle.getThemePhraseCoordinates().size(); j++)
        {
            uivec3 index = m_RefPuzzle.getThemePhraseCoordinates().at(j);
            GraphicsGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(index));
            item->setColor(Qt::yellow);
        }

        // set the numbers on the grid
        for(unsigned int k = 0; k < m_RefCrosswordEntries.size(); k++)
        {
            std::vector<std::pair<unsigned int, uivec3> > entryPairs = m_RefCrosswordEntries.at(k).getWordEntryStartingPositionPairs();

            for(unsigned int l = 0; l < entryPairs.size(); l++)
            {
                GraphicsGridItem* item = m_GraphicsGridItems.at(m_RefPuzzle.toGridIndex(entryPairs.at(l).second));
                item->setCrosswordEntryNumber(entryPairs.at(l).first);
            }
        }

    setSceneRect(itemsBoundingRect());

    m_SavedColours.clear();
    m_SelectedGridLocations.clear();

    repaintPuzzleGrid();
}

void Grid3DGraphicsScene::repaintPuzzleGrid()
{
    update();
}
