#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "puzzleloader.h"

class Puzzle3D;
class Grid3DGraphicsScene;
class WordTableModel;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow* ui;
    Puzzle3D* m_Puzzle;
    PuzzleLoader m_PuzzleLoader;
    Grid3DGraphicsScene* m_GraphicsScene;
    WordTableModel* m_WordTableModel;

    static const QString m_DefaultSaveFolder;

private slots:
    void loadFile();
    void saveFile();

signals:
    void puzzleLoaded();
};

#endif // MAINWINDOW_H
