#ifndef GRID3DGRAPHICSSCENE_H
#define GRID3DGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <vector>

#include "gridrect.h"

#include "uivec3.h"
#include "letter.h"
#include "crosswordentry3d.h"
#include "lettergrid.h"

class Grid3DGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    Grid3DGraphicsScene(LetterGrid* letters, std::vector<CrosswordEntry3D>* entries);
    ~Grid3DGraphicsScene();

    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    LetterGrid* m_RefWorkingGrid;
    std::vector<CrosswordEntry3D>* m_RefCrosswordEntries;

    bool m_IsGridBuilt;

    float m_SceneWidth;
    float m_SceneHeight;
    
signals:

public slots:
    void buildGrid();
    void addWordHighlight();
};

#endif // GRID3DGRAPHICSSCENE_H
