#ifndef IDLEREMINDER_H
#define IDLEREMINDER_H

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

#endif // IDLEREMINDER_H
