#ifndef GRID3DGRAPHICSVIEW_H
#define GRID3DGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

class Grid3DGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Grid3DGraphicsView(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event);
    
signals:

public slots:
    
};

#endif // GRID3DGRAPHICSVIEW_H
