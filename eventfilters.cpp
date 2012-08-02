#include "eventfilters.h"

#include <QEvent>

bool MouseAndKeyboardBlocker::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseMove:

        return true;
    }
    return QObject::eventFilter(obj, event);
}
