#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "puzzleloader.h"

class BCrossword3D;
class Grid3DGraphicsScene;
class WordTableModel;
class QItemSelection;
class QShortcut;
class ITextToSpeech;

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

    BCrossword3D* m_Puzzle;
    PuzzleLoader m_PuzzleLoader;
    Grid3DGraphicsScene* m_GraphicsScene;
    WordTableModel* m_WordTableModel;

    static const QString m_DefaultSaveFolder;
    static const QString m_HelpFileLocation;
    static const QString m_LicenseFileLocation;

    void createShortcuts();
    QShortcut* m_ExitShortcut;
    QShortcut* m_LoadShortcut;
    QShortcut* m_SaveShortcut;
    QShortcut* m_HelpShortcut;
    QShortcut* m_ScoreShortcut;
    QShortcut* m_FilePropertiesShortcut;

private slots:
    void loadFile();
    void saveFile();
    void showFileProperties();
    void exitConfirmation();
    void openHelp();
    void showAboutScreen();
    void scoreCrossword();
    void viewLicense();

    void showError(QString title, QString error);

signals:
    void puzzleLoaded();
};

#endif // MAINWINDOW_H
