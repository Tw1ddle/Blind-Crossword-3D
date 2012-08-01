#ifndef GRID3DGRAPHICSSCENE_H
#define GRID3DGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <vector>

#include "uivec3.h"
#include "letter.h"

class Grid3DGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    Grid3DGraphicsScene(uivec3* size, std::vector<Letter>* letters);

    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    void buildGrid();

    uivec3* m_RefGridSize;
    bool m_IsGridBuilt;
    std::vector<QPolygon> m_GridPolygons;
    std::vector<Letter>* m_RefWorkingLetters;

    float m_SceneWidth;
    float m_SceneHeight;
    
signals:
    
public slots:
};

#endif // GRID3DGRAPHICSSCENE_H
