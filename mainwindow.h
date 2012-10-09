#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "puzzleloader.h"
#include "puzzlebase.h"

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

    PuzzleLoader m_PuzzleLoader;
    PuzzleBase* m_Puzzle;
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
    QShortcut* m_CycleSpeechModeShortcut;
    QShortcut* m_ReadCrosswordThemePhraseShortcut;

    QString getIntroString() const;

private slots:
    void loadCrossword();
    void saveCrossword();
    void showFileProperties();
    void showFileThemePhrase();
    void exitConfirmation();
    void openHelp();
    void showAbout();
    void scoreCrossword();
    void viewLicense();
    void toggleGrid(bool hidden);
    void readCrosswordThemePhrase();
    void cycleSpeechMode();
    void cycleTableViewFilter();

    void checkIfPuzzleWasCompleted();

    void raiseError(QString title, QString error);

signals:
    void puzzleLoaded();
};

#endif // MAINWINDOW_H
