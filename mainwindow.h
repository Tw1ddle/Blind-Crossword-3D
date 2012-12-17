/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "crosswordloader.h"
#include "crosswordbase.h"

class GraphicalGridScene;
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

    CrosswordLoader m_PuzzleLoader;
    CrosswordBase m_Puzzle;

    GraphicalGridScene* m_GraphicsScene;
    WordTableModel* m_WordTableModel;
    QSortFilterProxyModel* m_ProxyModel;

    IdleReminder* m_IdleReminder;
    ClueReader* m_AdvancedClueReader;

    static const QString m_DefaultSaveFolder;
    static const QString m_HelpFileLocation;
    static const QString m_TutorialFileLocation;
    static const QString m_LicenseFileLocation;
    static const QString m_CalendarPuzzlesWebsiteAddressLocation;

    void createShortcuts();

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
    void openCalendarPuzzlesWebsite();
    void toggleGrid(bool hidden);

    void toggleApplicationOpenReminder();
    void onIdleReminderTimeout();

    void readCrosswordThemePhrase();
    void cycleSpeechMode();
    void cycleTableViewFilter();

    void stopSpeech();

    void advanceToNextWordInClue();
    void readCurrentWordInClue();

    void increaseSpeechRate();
    void decreaseSpeechRate();

    void raiseError(QString title, QString error);

signals:
    void puzzleLoaded();

private:
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
    QShortcut* m_IncreaseSpeechRateShortcut;
    QShortcut* m_DecreaseSpeechRateShortcut;
};

#endif // MAINWINDOW_H
