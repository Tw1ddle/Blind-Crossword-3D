/*! \brief Fires a repeating reminder that the program is still running if the user does not interact with the program.
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

#include <QObject>
#include <QTimer>
#include <QEvent>

class IdleReminder : public QObject
{
    Q_OBJECT
public:
    explicit IdleReminder(float frequency, QObject *parent = 0);
    ~IdleReminder();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    float m_Frequency;
    QTimer* m_Timer;

public slots:
    void onTimeOut();

signals:
    void timedOut();
};
