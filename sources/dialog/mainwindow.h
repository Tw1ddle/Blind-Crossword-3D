/*!
 * \section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * \brief The main window.
 *
 *  Responsible for setting up of shortcuts and connections between program components.
 *  Provides voiced feedback for most of the actions taken within the program.
 */

#pragma once

#include <QMainWindow>

#include "crossword/crosswordloader.h"
#include "crossword/crosswordbase.h"
#include "dialog/speechlogwidget.h"

class GraphicalGridScene;
class CrosswordEntryTableModel;
class QShortcut;
class QSortFilterProxyModel;

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

    CrosswordLoader m_CrosswordLoader;
    CrosswordBase m_Crossword;

    GraphicalGridScene* m_GraphicsScene;
    CrosswordEntryTableModel* m_TableModel;
    QSortFilterProxyModel* m_ProxyModel;

    ClueReader* m_ClueReader;

    // if we're debugging, add a text edit with everything the voice says
    #ifdef QT_DEBUG
        SpeechLogWidget m_SpeechLogWidget;
    #endif // QT_DEBUG

    void createShortcuts();

    QString getIntroString() const;

private slots:
    void loadCrossword();
    void saveCrossword();
    void printAnswers();
    void emailAnswers();
    void showFileProperties();
    void exitConfirmation();
    void openHelp();
    void openTutorial();
    void emailFeedback();
    void showAbout();
    void scoreCrossword();
    void viewLicense();
    void openCalendarPuzzlesWebsite();

    void cycleViewVisibility();

    void cycleSpeechMode();
    void cycleTableViewFilter();

    void stopSpeech();
    void readLastSpokenPhrase();

    void advanceToNextWordInClue();
    void readCurrentWordInClue();

    void increaseSpeechRate();
    void decreaseSpeechRate();

    void raiseError(QString title, QString error);

signals:
    void puzzleLoaded();

private:
    static const QString m_DefaultSaveFolder;
    static const QString m_HelpFileLocation;
    static const QString m_TutorialFileLocation;
    static const QString m_LicenseFileLocation;
    static const QString m_CalendarPuzzlesWebsiteAddressLocation;

    QShortcut* m_ExitShortcut;
    QShortcut* m_LoadShortcut;
    QShortcut* m_SaveShortcut;
    QShortcut* m_HelpShortcut;
    QShortcut* m_TutorialShortcut;

    QShortcut* m_EmailAnswersShortcut;
    QShortcut* m_EmailFeedbackShortcut;
    QShortcut* m_PrintAnswersShortcut;

    QShortcut* m_ScoreShortcut;
    QShortcut* m_RevealAnswerShortcut;
    QShortcut* m_FilePropertiesShortcut;
    QShortcut* m_FilterTableViewShortcut;
    QShortcut* m_CycleSpeechModeShortcut;
    QShortcut* m_ReadCrosswordThemePhraseShortcut;
    QShortcut* m_StopSpeechShortcut;
    QShortcut* m_ReadCurrentClueWordShortcut;
    QShortcut* m_AdvanceCurrentClueWordShortcut;
    QShortcut* m_IncreaseSpeechRateShortcut;
    QShortcut* m_DecreaseSpeechRateShortcut;
    QShortcut* m_ReadLastSpokenPhraseShortcut;
    QShortcut* m_CycleViewVisibilityShortcut;
};
