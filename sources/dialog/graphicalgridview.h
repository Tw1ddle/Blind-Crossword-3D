/*! \brief View for the graphical grid.
 *  Responsible for user navigation of the graphical grid, and managing the viewport of the graphical grid.
 */

#pragma once

#include <QGraphicsView>

class CrosswordEntry;

class GraphicalGridView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicalGridView(QWidget *parent = 0);

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    const static double cs_ZoomFactor;
    const static int cs_KeyboardZoomFactor;
    int m_NumScheduledScalings;

    void zoom(int x);

private slots:
    void zoomAnimationFinished();
    void scalingTime(qreal x);
};