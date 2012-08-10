#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "puzzleloader.h"

class BCrossword3D;
class Grid3DGraphicsScene;
class WordTableModel;
class QItemSelection;
class QShortcut;
class QSortFilterProxyModel;

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
    QSortFilterProxyModel* m_ProxyModel;

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
    QShortcut* m_FilterTableViewShortcut;

    QChar m_ExitShortcutKey;
    QChar m_LoadShortcutKey;
    QChar m_SaveShortcutKey;
    QChar m_HelpShortcutKey;
    QChar m_ScoreShortcutKey;
    QChar m_FilePropertiesShortcutKey;
    QChar m_FilterTableViewShortcutKey;

    QString getIntroString();

private slots:
    void loadCrossword();
    void saveCrossword();
    void showFileProperties();
    void exitConfirmation();
    void openHelp();
    void showAbout();
    void scoreCrossword();
    void viewLicense();
    void toggleGrid(bool hidden);

    void cycleTableViewFilter();

    void showError(QString title, QString error);

signals:
    void puzzleLoaded();
};

#endif // MAINWINDOW_H
