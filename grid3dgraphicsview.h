#ifndef GRID3DGRAPHICSVIEW_H
#define GRID3DGRAPHICSVIEW_H

#include <QGraphicsView>

class CrosswordEntry3D;

class Grid3DGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Grid3DGraphicsView(QWidget *parent = 0);

protected:
    virtual void wheelEvent(QWheelEvent *event);

private:
    const static double wheelZoomFactor;
    int m_NumScheduledScalings;

private slots:
    void updateSceneRect(const QRectF &rect);
    void zoomAnimationFinished();
    void scalingTime(qreal x);
    void print();
    
signals:
    void crosswordEntryPressed(CrosswordEntry3D& entry);
};

#endif // GRID3DGRAPHICSVIEW_H
