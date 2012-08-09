#include "grid3dgraphicsscene.h"

#include <QPainter>
#include <QList>
#include <QGraphicsItem>
#include <assert.h>

#include "graphicsgriditem.h"

Grid3DGraphicsScene::Grid3DGraphicsScene(LetterGrid* letters, std::vector<CrosswordEntry3D>* entries) :
    QGraphicsScene(), m_RefGrid(letters), m_RefCrosswordEntries(entries)
{
}

Grid3DGraphicsScene::~Grid3DGraphicsScene()
{
}

void Grid3DGraphicsScene::build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber)
{
    for(unsigned int y = 0; y < yDim; y++)
    {
        for(unsigned int x = 0; x < xDim; x++)
        {
            unsigned int index = uivec3(x, y, gridNumber).toXYZGridIndex(uivec3(xDim, yDim, 0));
            Letter letter = m_RefGrid->getLetterAt(index);

            GraphicsGridItem* item = new GraphicsGridItem(letter, gridNumber);
            item->setPos(QPointF(x * GraphicsGridItem::s_Size + offset.getX(), y * GraphicsGridItem::s_Size));

            if(letter.getChar() != QChar())
            {
                item->setColor(Qt::white);
            }
            else
            {
                item->setColor(QColor(20, 20, 20));
            }

            addItem(item);
        }
    }
}

void Grid3DGraphicsScene::buildPuzzleGrid()
{
    this->clear();

    for(unsigned int z = 0; z < m_RefGrid->getDimensions().getZ(); z++)
    {
        build2DGrid(m_RefGrid->getDimensions().getX(), m_RefGrid->getDimensions().getY(), uivec3(z * (m_RefGrid->getDimensions().getZ()) * GraphicsGridItem::s_Size, 0, 0), z);
    }

    for(unsigned int i = 0; i < m_RefCrosswordEntries->size(); i++)
    {
        unsigned int entryNumber = m_RefCrosswordEntries->at(i).getEntryNumber();
        uivec3 startingPos =  m_RefCrosswordEntries->at(i).getStartingPosition();
        unsigned int index = m_RefCrosswordEntries->at(i).getStartingPosition().toYXZGridIndex(m_RefGrid->getDimensions());
        GraphicsGridItem* item = static_cast<GraphicsGridItem*> (items().at(items().size() - 1 - index));
        item->setCrosswordEntryNumber(entryNumber);
    }

    setSceneRect(itemsBoundingRect());
}

void Grid3DGraphicsScene::highlightSelection()
{
}
