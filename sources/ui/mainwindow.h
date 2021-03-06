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

#include "crossword/crosswordbase.h"
#include "loader/crosswordloader.h"
#include "ui/speechlogwidget.h"

namespace Ui {

class MainWindow;

}

class QShortcut;
class QSortFilterProxyModel;

namespace ui {

class GraphicalGridScene;
class CrosswordEntryTableModel;

}

namespace crossword {

class ClueReader;

}

namespace ui {

class MainWindow : public QMainWindow {
    Q_OBJECT
    
  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

  signals:
    void puzzleLoaded();

  protected:
    void closeEvent(QCloseEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);

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

    Ui::MainWindow* ui;

    loader::CrosswordLoader m_crosswordLoader;
    crossword::CrosswordBase m_crossword;

    GraphicalGridScene* m_graphicsScene;
    CrosswordEntryTableModel* m_tableModel;
    QSortFilterProxyModel* m_proxyModel;

    crossword::ClueReader* m_clueReader;

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

}
