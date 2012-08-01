#include "grid3dgraphicsscene.h"

#include <QPainter>
#include <QTimer>

Grid3DGraphicsScene::Grid3DGraphicsScene(uivec3* size, std::vector<Letter>* letters) :
    QGraphicsScene(), m_RefGridSize(size), m_RefWorkingLetters(letters), m_IsGridBuilt(false)
{
}

void Grid3DGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    m_SceneWidth = rect.size().width();
    m_SceneHeight = rect.size().height();

    buildGrid();

    for(unsigned int i = 0; i < m_GridPolygons.size(); i++)
    {
        if(i < m_RefWorkingLetters->size())
        {
            if(QString(m_RefWorkingLetters->at(i).getLetter()) == QString(" "))
            {
                painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
            }
            else
            {
                painter->setBrush(QBrush(Qt::NoBrush));
            }
        }

        painter->drawPolygon(m_GridPolygons.at(i));

        if(i < m_RefWorkingLetters->size())
        {
            painter->drawText(m_GridPolygons.at(i).boundingRect(), m_RefWorkingLetters->at(i).getLetter());
        }
    }

     QTimer::singleShot(150, this, SLOT(update()));
}

void Grid3DGraphicsScene::buildGrid()
{
    m_IsGridBuilt = true;
    m_GridPolygons.clear();

    float puzzleWidth = m_SceneWidth * 0.9f;
    float puzzleHeight = m_SceneHeight * 0.8f;

    float squareWidth = puzzleWidth/((float)m_RefGridSize->getX());
    float squareHeight = puzzleHeight/((float)m_RefGridSize->getY() * (float)m_RefGridSize->getZ());

    float numGrids = m_RefGridSize->getZ();

    float puzzleWidthOffset = m_SceneWidth * 0.05f;
    float puzzleHeightOffset = m_SceneHeight * 0.05f;

    float gridSpacing = puzzleHeightOffset/2.0f;

    for(unsigned int z = 0; z < m_RefGridSize->getZ(); z++)
    {
        for(unsigned int y = 0 ; y < m_RefGridSize->getY(); y++)
        {
            for(unsigned int x = 0; x < m_RefGridSize->getX(); x++)
            {
                float sqX = puzzleWidthOffset + squareWidth * x;
                float sqY = puzzleHeightOffset + squareHeight * y;

                float x1 = sqX;
                float y1 = sqY;

                float x2 = sqX;
                float y2 = sqY + squareHeight;

                float x3 = sqX + squareWidth;
                float y3 = sqY + squareHeight;

                float x4 = sqX + squareWidth;
                float y4 = sqY;

                QVector<QPoint> points;
                points.push_back(QPoint(x1, y1));
                points.push_back(QPoint(x2, y2));
                points.push_back(QPoint(x3, y3));
                points.push_back(QPoint(x4, y4));

                m_GridPolygons.push_back(QPolygon(points));
            }
        }
        puzzleHeightOffset += puzzleHeight/numGrids + gridSpacing;
    }
}
