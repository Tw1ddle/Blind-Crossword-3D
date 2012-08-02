#ifndef EVENTFILTERS_H
#define EVENTFILTERS_H

#include <QObject>

class MouseAndKeyboardBlocker : public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // EVENTFILTERS_H
