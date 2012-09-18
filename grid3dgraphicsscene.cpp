#include "grid3dgraphicsscene.h"

#include <QGraphicsItem>
#include <QDir>
#include <QPainter>

#include "graphicsgriditem.h"
#include "lettergrid.h"

Grid3DGraphicsScene::Grid3DGraphicsScene(LetterGrid* letters, std::vector<CrosswordEntry3D>* entries, QPixmap* backgroundImage) :
    QGraphicsScene(), m_RefGrid(letters), m_RefCrosswordEntries(entries), m_RefBackgroundImage(backgroundImage), m_PreviousSelectedCrosswordEntryNumber(1u), m_CurrentlySelectedCrosswordEntryNumber(1u)
{
}

void Grid3DGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->drawPixmap(rect.left(), rect.top(), rect.width(), rect.height(), *m_RefBackgroundImage);
}

void Grid3DGraphicsScene::build2DGrid(unsigned int xDim, unsigned int yDim, uivec3 offset, unsigned int gridNumber)
{
    for(unsigned int x = 0; x < xDim; x++)
    {
        for(unsigned int y = 0; y < yDim; y++)
        {
            unsigned int index = uivec3(x, y, gridNumber).toGridIndex(uivec3(xDim, yDim, 0));
            Letter letter = m_RefGrid->getLetterAt(index);

            GraphicsGridItem* item = new GraphicsGridItem(letter, gridNumber);
            item->setPos(QPointF(y * GraphicsGridItem::sc_Size + offset.getY(), x * GraphicsGridItem::sc_Size));

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

    m_CurrentlySelectedCrosswordEntryNumber = 1;
    m_PreviousSelectedCrosswordEntryNumber = 1;

    QString path = "/Backgrounds";
    path.append("/").append("sunset.jpg");
    QDir dir;
    QString imagePath;
    if(dir.exists(dir.absolutePath().append(path)))
    {
        imagePath = dir.absolutePath().append(path);
    }
    m_RefBackgroundImage = new QPixmap(imagePath);

    for(unsigned int z = 0; z < m_RefGrid->getDimensions().getZ(); z++)
    {
        build2DGrid(m_RefGrid->getDimensions().getX(), m_RefGrid->getDimensions().getY(), uivec3(0, z * (m_RefGrid->getDimensions().getZ()) * GraphicsGridItem::sc_Size, 0), z);
    }

    for(unsigned int i = 0; i < m_RefCrosswordEntries->size(); i++)
    {
        unsigned int entryNumber = m_RefCrosswordEntries->at(i).getEntryNumber();
        uivec3 startingPos =  m_RefCrosswordEntries->at(i).getStartingPosition();
        unsigned int index = m_RefCrosswordEntries->at(i).getStartingPosition().toGridIndex(m_RefGrid->getDimensions());
        GraphicsGridItem* item = static_cast<GraphicsGridItem*> (items().at(items().size() - index - 1));
        item->setCrosswordEntryNumber(entryNumber);
    }

    highlightSelection(m_CurrentlySelectedCrosswordEntryNumber);

    setSceneRect(itemsBoundingRect());
}

void Grid3DGraphicsScene::highlightSelection(unsigned int selectedCrosswordEntryNumber)
{
    m_CurrentlySelectedCrosswordEntryNumber = selectedCrosswordEntryNumber;

    for(unsigned int i = 0; i < items().size(); i++)
    {
        GraphicsGridItem* currentItem = static_cast<GraphicsGridItem*>(items().at(i));

        if(currentItem->getCrosswordEntryNumber().toUInt() == m_PreviousSelectedCrosswordEntryNumber)
        {
            currentItem->setColor(Qt::white);
        }

        if(currentItem->getCrosswordEntryNumber().toUInt() == selectedCrosswordEntryNumber)
        {
            currentItem->setColor(Qt::cyan);
        }
    }

    update();

    m_PreviousSelectedCrosswordEntryNumber = selectedCrosswordEntryNumber;
}
