#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "puzzleloader.h"

class Puzzle3D;
class Grid3DGraphicsScene;
class WordTableModel;
class QItemSelection;
class QShortcut;

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

protected:
    void closeEvent(QCloseEvent* event);
    
private:
    Ui::MainWindow* ui;
    Puzzle3D* m_Puzzle;
    PuzzleLoader m_PuzzleLoader;
    Grid3DGraphicsScene* m_GraphicsScene;
    WordTableModel* m_WordTableModel;

    static const QString m_DefaultSaveFolder;

    void createShortcuts();
    QShortcut* m_ExitShortcut;
    QShortcut* m_LoadShortcut;
    QShortcut* m_SaveShortcut;
    QShortcut* m_HelpShortcut;

private slots:
    void loadFile();
    void saveFile();
    void exitConfirmation();
    void openHelp();

    void tableSelectionChanged(const QItemSelection&);

signals:
    void puzzleLoaded();
};

#endif // MAINWINDOW_H
