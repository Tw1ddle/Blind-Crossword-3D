#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //todo - parse command line arguments for opening associated crossword files etc

    w.show();
    
    return a.exec();
}
