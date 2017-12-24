#include "graphicalgridview.h"

#include <QResizeEvent>
#include <QTimeLine>
#include <QPrinter>
#include <QPrintDialog>

#include "shortcutkeys.h"

const double GraphicalGridView::cs_ZoomFactor = 1.25;
const int GraphicalGridView::cs_KeyboardZoomFactor = 75;

GraphicalGridView::GraphicalGridView(QWidget *parent) :
    QGraphicsView(parent)
{
    m_NumScheduledScalings = 0;
}

void GraphicalGridView::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta()/8;

    zoom(numDegrees);
}

void GraphicalGridView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);

    if(event->key() == ShortcutKeys::zoomInKey)
    {
        zoom(cs_KeyboardZoomFactor);
    }
    else if(event->key() == ShortcutKeys::zoomOutKey)
    {
        zoom(-cs_KeyboardZoomFactor);
    }
}

void GraphicalGridView::zoom(int x)
{
    int numSteps = x/15;
    m_NumScheduledScalings += numSteps;

    if(m_NumScheduledScalings * numSteps < 0)
    {
        m_NumScheduledScalings = numSteps;
    }

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(zoomAnimationFinished()));
    anim->start();
}

// http://qt-project.org/wiki/SmoothZoomInQGraphicsView
void GraphicalGridView::zoomAnimationFinished()
{
    if(m_NumScheduledScalings > 0)
    {
        m_NumScheduledScalings--;
    }
    else
    {
        m_NumScheduledScalings++;
        sender()->~QObject();
    }
}

void GraphicalGridView::scalingTime(qreal x)
{
    Q_UNUSED(x);

    float factor = 1.0f + static_cast<float>(m_NumScheduledScalings) / 450.0f;
    scale(factor, factor);
}
