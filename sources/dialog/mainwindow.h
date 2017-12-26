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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    
  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

  signals:
    void puzzleLoaded();

  protected:
    void closeEvent(QCloseEvent* event);

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

  private:
    // if we're debugging, add a text edit with everything the voice says
#ifdef QT_DEBUG
    SpeechLogWidget m_SpeechLogWidget;
#endif // QT_DEBUG

    void createShortcuts();
    QString getIntroString() const;

    static const QString DEFAULT_SAVE_FOLDER;
    static const QString HELP_FILE_LOCATION;
    static const QString TUTORIAL_FILE_LOCATION;
    static const QString LICENSE_FILE_LOCATION;
    static const QString WEBSITE_ADDRESS_LOCATION;

    Ui::MainWindow* ui;

    CrosswordLoader m_crosswordLoader;
    CrosswordBase m_crossword;

    GraphicalGridScene* m_graphicsScene;
    CrosswordEntryTableModel* m_tableModel;
    QSortFilterProxyModel* m_proxyModel;

    ClueReader* m_clueReader;

    QShortcut* m_exitShortcut;
    QShortcut* m_loadShortcut;
    QShortcut* m_saveShortcut;
    QShortcut* m_helpShortcut;
    QShortcut* m_tutorialShortcut;

    QShortcut* m_emailAnswersShortcut;
    QShortcut* m_emailFeedbackShortcut;
    QShortcut* m_printAnswersShortcut;

    QShortcut* m_scoreShortcut;
    QShortcut* m_revealAnswerShortcut;
    QShortcut* m_filePropertiesShortcut;
    QShortcut* m_filterTableViewShortcut;
    QShortcut* m_cycleSpeechModeShortcut;
    QShortcut* m_readCrosswordThemePhraseShortcut;
    QShortcut* m_stopSpeechShortcut;
    QShortcut* m_readCurrentClueWordShortcut;
    QShortcut* m_advanceCurrentClueWordShortcut;
    QShortcut* m_increaseSpeechRateShortcut;
    QShortcut* m_decreaseSpeechRateShortcut;
    QShortcut* m_readLastSpokenPhraseShortcut;
    QShortcut* m_cycleViewVisibilityShortcut;
};
