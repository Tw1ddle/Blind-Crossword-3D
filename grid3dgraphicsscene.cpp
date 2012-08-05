#include "grid3dgraphicsscene.h"

#include <QPainter>
#include <QTimer>
#include <QList>
#include <QGraphicsItem>
#include <assert.h>

Grid3DGraphicsScene::Grid3DGraphicsScene(LetterGrid* letters, std::vector<CrosswordEntry3D>* entries) :
    QGraphicsScene(), m_RefWorkingGrid(letters), m_RefCrosswordEntries(entries), m_IsGridBuilt(false)
{
}

Grid3DGraphicsScene::~Grid3DGraphicsScene()
{
}

void Grid3DGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    m_SceneWidth = rect.size().width();
    m_SceneHeight = rect.size().height();
    QPen pen;
    pen.setWidth(0);
    pen.setCosmetic(true);
    painter->setPen(pen);

    for(unsigned int i = 0; i < items().size(); i++)
    {
        if(m_RefWorkingGrid->getLetterAtIndex(i).getChar() == QChar())
        {
            painter->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
        }

        painter->drawText(items().at(i)->boundingRect(), Qt::AlignCenter, m_RefWorkingGrid->getLetterAtIndex(items().size() - 1 - i).getChar());
    }

     QTimer::singleShot(100, this, SLOT(update()));
}

void Grid3DGraphicsScene::buildGrid()
{
    m_IsGridBuilt = true;

    this->clear();

    float puzzleWidth = m_SceneWidth * 0.9f;
    float puzzleHeight = m_SceneHeight * 0.8f;

    float squareWidth = puzzleWidth/((float)m_RefWorkingGrid->getDimensions().getX());
    float squareHeight = puzzleHeight/((float)m_RefWorkingGrid->getDimensions().getY() * (float)m_RefWorkingGrid->getDimensions().getZ());

    float numGrids = m_RefWorkingGrid->getDimensions().getZ();

    float puzzleWidthOffset = m_SceneWidth * 0.05f;
    float puzzleHeightOffset = m_SceneHeight * 0.05f;

    float gridSpacing = puzzleHeightOffset/2.0f;

    for(unsigned int z = 0; z < m_RefWorkingGrid->getDimensions().getZ(); z++)
    {
        for(unsigned int y = 0 ; y < m_RefWorkingGrid->getDimensions().getY(); y++)
        {
            for(unsigned int x = 0; x < m_RefWorkingGrid->getDimensions().getX(); x++)
            {
                float sqX = puzzleWidthOffset + squareWidth * x;
                float sqY = puzzleHeightOffset + squareHeight * y;

                this->addRect(sqX, sqY + squareHeight/2.0f, squareWidth, squareHeight, QPen(), QBrush(Qt::NoBrush));
            }
        }
        puzzleHeightOffset += puzzleHeight/numGrids + gridSpacing;
    }
}

void Grid3DGraphicsScene::addWordHighlight()
{
}
