#include "graphicalgridview.h"

#include <QResizeEvent>
#include <QTimeLine>
#include <QPrinter>
#include <QPrintDialog>

#include "shortcutkeys.h"

const double GraphicalGridView::ZOOM_FACTOR = 1.25;
const int GraphicalGridView::KEYBOARD_ZOOM_FACTOR = 75;

GraphicalGridView::GraphicalGridView(QWidget *parent) :
    QGraphicsView(parent)
{
    m_numScheduledScalings = 0;
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
        zoom(KEYBOARD_ZOOM_FACTOR);
    }
    else if(event->key() == ShortcutKeys::zoomOutKey)
    {
        zoom(-KEYBOARD_ZOOM_FACTOR);
    }
}

void GraphicalGridView::zoom(int x)
{
    int numSteps = x/15;
    m_numScheduledScalings += numSteps;

    if(m_numScheduledScalings * numSteps < 0)
    {
        m_numScheduledScalings = numSteps;
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
    if(m_numScheduledScalings > 0)
    {
        m_numScheduledScalings--;
    }
    else
    {
        m_numScheduledScalings++;
        sender()->~QObject();
    }
}

void GraphicalGridView::scalingTime(qreal x)
{
    Q_UNUSED(x);

    float factor = 1.0f + static_cast<float>(m_numScheduledScalings) / 450.0f;
    scale(factor, factor);
}
