#include "grid3dgraphicsview.h"

Grid3DGraphicsView::Grid3DGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void Grid3DGraphicsView::resizeEvent(QResizeEvent *event)
{
    if (scene())
    {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        QGraphicsView::resizeEvent(event);
    }
}
