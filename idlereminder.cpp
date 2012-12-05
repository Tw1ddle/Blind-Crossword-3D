#include "idlereminder.h"

IdleReminder::IdleReminder(float frequency, QObject *parent) :
    QObject(parent), m_Frequency(frequency)
{
    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    m_Timer->start(m_Frequency);
}

IdleReminder::~IdleReminder()
{
    delete m_Timer;
}

bool IdleReminder::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::KeyPress || ev->type() == QEvent::MouseMove)
    {
        m_Timer->start(m_Frequency);
    }

    return QObject::eventFilter(obj, ev);
}

void IdleReminder::onTimeOut()
{
    emit timedOut();
}
