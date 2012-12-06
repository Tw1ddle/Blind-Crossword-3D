#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "puzzleloader.h"
#include "puzzlebase.h"

class Grid3DGraphicsScene;
class WordTableModel;
class QShortcut;
class QSortFilterProxyModel;

class IdleReminder;
class ClueReader;

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
    PuzzleBase m_Puzzle;

    Grid3DGraphicsScene* m_GraphicsScene;
    WordTableModel* m_WordTableModel;
    QSortFilterProxyModel* m_ProxyModel;

    IdleReminder* m_IdleReminder;
    ClueReader* m_AdvancedClueReader;


    static const QString m_DefaultSaveFolder;
    static const QString m_HelpFileLocation;
    static const QString m_LicenseFileLocation;
    static const QString m_EmailAddressFileLocation;
    static const QString m_PostalAddressFileLocation;

    void createShortcuts();
    QShortcut* m_ExitShortcut;
    QShortcut* m_LoadShortcut;
    QShortcut* m_SaveShortcut;
    QShortcut* m_HelpShortcut;
    QShortcut* m_ScoreShortcut;
    QShortcut* m_RevealAnswerShortcut;
    QShortcut* m_FilePropertiesShortcut;
    QShortcut* m_FilterTableViewShortcut;
    QShortcut* m_CycleSpeechModeShortcut;
    QShortcut* m_ReadCrosswordThemePhraseShortcut;
    QShortcut* m_StopSpeechShortcut;

    QShortcut* m_ApplicationOpenReminderShortcut;

    QShortcut* m_ReadCurrentClueWordShortcut;
    QShortcut* m_AdvanceCurrentClueWordShortcut;

    QString getIntroString() const;

    bool m_ApplicationOpenReminderEnabled;

private slots:
    void loadCrossword();
    void saveCrossword();
    void printCrossword();
    void emailCrossword();
    void showFileProperties();
    void exitConfirmation();
    void openHelp();
    void emailFeedback();
    void showAbout();
    void scoreCrossword();
    void viewLicense();
    void toggleGrid(bool hidden);

    void toggleApplicationOpenReminder();
    void onIdleReminderTimeout();

    void readCrosswordThemePhrase();
    void cycleSpeechMode();
    void cycleTableViewFilter();

    void stopSpeech();
    bool changeSpeechRate(float change);

    void advanceToNextWordInClue();
    void readCurrentWordInClue();

    void checkIfPuzzleWasCompleted();

    void raiseError(QString title, QString error);

signals:
    void puzzleLoaded();
};

#endif // MAINWINDOW_H
