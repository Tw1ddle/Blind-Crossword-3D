#ifndef QUITMESSAGEBOX_H
#define QUITMESSAGEBOX_H

#include <QMessageBox>

class QuitMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit QuitMessageBox(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // QUITMESSAGEBOX_H
