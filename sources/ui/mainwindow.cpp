#include "ui/mainwindow.h"

#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMimeData>
#include <QShortcut>
#include <QSortFilterProxyModel>
#include <QUrl>

#include "assets/assetpaths.h"
#include "controls/controls.h"
#include "crossword/cluereader.h"
#include "crossword/crosswordbase.h"
#include "email/emailer.h"
#include "printing/printer.h"
#include "tts/itexttospeech.h"
#include "ui/crosswordentrytablemodel.h"
#include "ui/filedialog.h"
#include "ui/graphicalgridscene.h"
#include "ui/quitdialog.h"
#include "util/util.h"
#include "version/version.h"

namespace {

bool loadCrosswordPuzzle(const QString& path, crossword::CrosswordBase& crossword,
                         loader::CrosswordLoader& loader)
{
    if (path.isNull()) {
        return false;
    }

    QFileInfo fileInfo(path);
    QString extension = fileInfo.suffix();
    return loader.loadPuzzle(crossword, path, extension);
}

}

namespace ui {

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(version::getApplicationName().append(" (Version ").append(
                       version::getApplicationVersion().append(")")));
    setWindowIcon(QIcon(":/icon.ico"));

    createShortcuts();

    m_graphicsScene = new ui::GraphicalGridScene(m_crossword);
    ui->graphicsView->setScene(m_graphicsScene);

    m_tableModel = new ui::CrosswordEntryTableModel(m_crossword, m_crossword.getRefEntries());
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_tableModel);
    ui->wordTableView->setModel(m_proxyModel);

    m_clueReader = new crossword::ClueReader();

    connect(this, &MainWindow::puzzleLoaded, [this]() {
        m_tableModel->crosswordEntriesChanged();
        ui->wordTableView->sortEntriesByIdentifierAscending();
    });
    connect(this, &MainWindow::puzzleLoaded, [this]() {
        ui->wordTableView->setFocus(Qt::OtherFocusReason);
    });
    connect(this, &MainWindow::puzzleLoaded, m_graphicsScene, [this]() {
        m_graphicsScene->buildPuzzleGrid();
    });

    connect(m_tableModel, &ui::CrosswordEntryTableModel::conflictingWordError, ui->wordTableView,
            &ui::CrosswordEntryTableViewController::conflictingWordError);

    connect(ui->wordTableView, &ui::CrosswordEntryTableViewController::modelIndexChanged,
            m_tableModel, &ui::CrosswordEntryTableModel::tableViewSelectionChanged);

    //connect(ui->wordTableView, &ui::CrosswordEntryTableViewController::guessSubmitted, this, &MainWindow::checkIfPuzzleWasCompleted);

    connect(ui->wordTableView, &ui::CrosswordEntryTableViewController::guessSubmitted, m_tableModel,
            &ui::CrosswordEntryTableModel::enterGuess);
    connect(ui->wordTableView, &ui::CrosswordEntryTableViewController::guessAmendationRequested,
            m_tableModel,
            &ui::CrosswordEntryTableModel::amendGuess);
    connect(ui->wordTableView, &ui::CrosswordEntryTableViewController::guessErasureRequested,
            m_tableModel,
            &ui::CrosswordEntryTableModel::eraseGuess);

    connect(m_tableModel, &ui::CrosswordEntryTableModel::guessValidated, ui->wordTableView,
            &ui::CrosswordEntryTableViewController::reportGuessAccepted);
    connect(m_tableModel, &ui::CrosswordEntryTableModel::guessAmended, ui->wordTableView,
            &ui::CrosswordEntryTableViewController::reportGuessAmended);
    connect(m_tableModel, &ui::CrosswordEntryTableModel::guessErased, ui->wordTableView,
            &ui::CrosswordEntryTableViewController::reportGuessErased);
    connect(m_tableModel, &ui::CrosswordEntryTableModel::guessAmendationRequestRejected,
            ui->wordTableView,
            &ui::CrosswordEntryTableViewController::reportGuessAmendationRejected);

    connect(m_tableModel, &ui::CrosswordEntryTableModel::guessValidated, m_graphicsScene,
            &ui::GraphicalGridScene::repaintPuzzleGrid);
    connect(m_tableModel, &ui::CrosswordEntryTableModel::guessAmended, m_graphicsScene,
            &ui::GraphicalGridScene::repaintPuzzleGrid);
    connect(m_tableModel, &ui::CrosswordEntryTableModel::guessErased, m_graphicsScene,
            &ui::GraphicalGridScene::repaintPuzzleGrid);

    connect(m_tableModel, &CrosswordEntryTableModel::crosswordEntrySelectionChanged, m_graphicsScene,
            &ui::GraphicalGridScene::highlightSelection);

    //connect(&m_crosswordLoader, &loader::CrosswordLoader::loaderError, this, &MainWindow::showError);

    connect(m_tableModel, &CrosswordEntryTableModel::crosswordEntrySelectionChanged, m_clueReader,
            &crossword::ClueReader::setText);

    tts::ITextToSpeech::instance().speak(getIntroString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCrossword()
{
    QDir dir(assets::getExternalCrosswordsFolderPath());

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    tts::ITextToSpeech::instance().speak("Opening crossword list. Use the arrow keys to navigate the list.");

    QStringList fileNameFilter;
    fileNameFilter.push_back(QString("*.xwc"));
    fileNameFilter.push_back(QString("*.xwc3d"));
    fileNameFilter.push_back(QString("*.xwcr3d"));

    FileDialog crosswordDialog(this,
                               "Choose a crossword to load",
                               dir.absolutePath(),
                               "Crossword Grid 3D(*.xwc3d);; Crossword Disc 3D(*.xwcr3d);;Crossword Compiler (*.xwc)",
                               fileNameFilter);

    QString path;

    if (crosswordDialog.exec()) {
        path = crosswordDialog.getSelectedFilePath();
    }

    if (loadCrosswordPuzzle(path, m_crossword, m_crosswordLoader)) {
        emit puzzleLoaded();
        QFileInfo fileInfo(path);
        tts::ITextToSpeech::instance().speak(fileInfo.completeBaseName().append(" was loaded."));
    } else {
        tts::ITextToSpeech::instance().speak("No crossword was selected.");
    }
}

void MainWindow::saveCrossword()
{
    QDir dir(assets::getExternalCrosswordsFolderPath());

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString path = dir.absolutePath()
                   .append("/")
                   .append(m_crossword.getTitle())
                   .append(".")
                   .append(m_crossword.getFormat());

    QFileInfo fileInfo(path);

    unsigned int extraTag = 1;
    QString separatorTag = QString("_");

    while (fileInfo.exists(path)) {
        QString updatedFileName = fileInfo.baseName()
                                  .append(separatorTag)
                                  .append(QString::number(extraTag))
                                  .append(".")
                                  .append(m_crossword.getFormat());

        path = dir.absolutePath()
               .append("/")
               .append(updatedFileName);

        extraTag++;
    }

    QFileInfo updatedFileInfo(path);

    if (m_crosswordLoader.savePuzzle(m_crossword, path, m_crossword.getFormat())) {
        tts::ITextToSpeech::instance().speak(QString("Crossword was saved as: ")
                                             .append(updatedFileInfo.fileName())
                                             .append(" in folder: ")
                                             .append(updatedFileInfo.filePath()));
    } else {
        tts::ITextToSpeech::instance().speak(
            QString("Crossword could not be saved. Are you sure a crossword is open?"));
    }
}

void MainWindow::cycleSpeechMode()
{
    const static unsigned int cs_NumSpeechModes = 2;
    static unsigned int s_SpeechMode = 0;

    switch (s_SpeechMode) {
        case 0:
            tts::ITextToSpeech::instance().speak("Spelling speech mode activated. Words will be spelled out letter by letter.");
            tts::ITextToSpeech::instance().setMode(tts::SPEECH_MODES::spellingOutSpeech);
            break;

        case 1:
            tts::ITextToSpeech::instance().setMode(tts::SPEECH_MODES::normalSpeech);
            tts::ITextToSpeech::instance().speak("Regular speech mode activated. Words will be read normally.");
            break;
    }

    s_SpeechMode++;

    if (s_SpeechMode >= cs_NumSpeechModes) {
        s_SpeechMode = 0;
    }
}

void MainWindow::cycleTableViewFilter()
{
    const static unsigned int cs_NumFilters = 4;
    static unsigned int s_Filter = 0;

    // filters anything that contains anything but any number of repetitions of "."
    QRegExp showUnstarted = QRegExp("^[\\.]*$");

    // filters anything that does not contain at least one non "." and a "."
    QRegExp showPartial = QRegExp("[^\\.].*[\\.]|[\\.].*[^\\.]");

    // filters anything that contains any number of repetitions of "."
    QRegExp showCompleted = QRegExp("^[^\\.]*$");

    // filters nothing
    QRegExp showAll = QRegExp("");

    switch (s_Filter) {
        case 0:
            m_proxyModel->setFilterRegExp(showUnstarted);
            m_proxyModel->setFilterKeyColumn(tableheader::wordColumnId);
            tts::ITextToSpeech::instance().speak("Showing unstarted crossword entries.");
            break;

        case 1:
            m_proxyModel->setFilterRegExp(showCompleted);
            m_proxyModel->setFilterKeyColumn(tableheader::wordColumnId);
            tts::ITextToSpeech::instance().speak("Showing completed crossword entries.");
            break;

        case 2:
            m_proxyModel->setFilterRegExp(showPartial);
            m_proxyModel->setFilterKeyColumn(tableheader::wordColumnId);
            tts::ITextToSpeech::instance().speak("Showing partially completed crossword entries.");
            break;

        case 3:
            m_proxyModel->setFilterRegExp(showAll);
            m_proxyModel->setFilterKeyColumn(tableheader::wordColumnId);
            tts::ITextToSpeech::instance().speak("Filtering disabled.");
            break;
    }

    s_Filter++;

    if (s_Filter >= cs_NumFilters) {
        s_Filter = 0;
    }
}

void MainWindow::cycleViewVisibility()
{
    const static unsigned int cs_NumVisibilities = 3;
    static unsigned int s_Visibility = 0;

    switch (s_Visibility) {
        case 0:
            tts::ITextToSpeech::instance().speak("Grid shown, clues hidden.");
            ui->wordTableView->setHidden(true);
            ui->graphicsView->setHidden(false);
            break;

        case 1:
            tts::ITextToSpeech::instance().speak("Clues shown, grid hidden.");
            ui->wordTableView->setHidden(false);
            ui->graphicsView->setHidden(true);
            break;

        case 2:
            tts::ITextToSpeech::instance().speak("Clues and grid shown.");
            ui->wordTableView->setHidden(false);
            ui->graphicsView->setHidden(false);
            break;

        default:
            break;
    }

    s_Visibility++;

    if (s_Visibility >= cs_NumVisibilities) {
        s_Visibility = 0;
    }
}

void MainWindow::viewLicense()
{
    if (util::openUrl(assets::getLicenseWebsiteAddress())) {
        tts::ITextToSpeech::instance().speak("Opening license document in web browser. Use your screen reader to read the license document.");
    } else {
        tts::ITextToSpeech::instance().speak("Error, could not open license document.");
    }
}

void MainWindow::openHelp()
{
    if (util::openUrl(assets::getHelpWebsiteAddress())) {
        tts::ITextToSpeech::instance().speak("Opening help page in web browser. Use your screen reader to read the help page.");
    } else {
        tts::ITextToSpeech::instance().speak("Error, could not open help page.");
    }
}

void MainWindow::openTutorial()
{
    if (util::openUrl(assets::getTutorialWebsiteAddress())) {
        tts::ITextToSpeech::instance().speak("Opening guide page in web browser. Use your screen reader to read the guide page.");
    } else {
        tts::ITextToSpeech::instance().speak("Error, could not open guide page.");
    }
}

void MainWindow::openCalendarPuzzlesWebsite()
{
    if (util::openUrl(assets::getWebsiteAddress())) {
        tts::ITextToSpeech::instance().speak("Opening Calendar Puzzles website in web browser. Use your screen reader to read the website.");
    } else {
        tts::ITextToSpeech::instance().speak("Error, could not open Calendar Puzzles website.");
    }
}

void MainWindow::printAnswers()
{
    tts::ITextToSpeech::instance().speak("Opening a print dialog for printing your answers. Use your screen reader to work with the dialog.");

    printing::Printer printer;
    QString result = printer.openPrintDialog(m_crossword, this);
    tts::ITextToSpeech::instance().speak(result);
}

void MainWindow::emailAnswers()
{
    email::Emailer emailer;

    if (emailer.openSendResultsEmail(m_crossword)) {
        tts::ITextToSpeech::instance().speak("Opening an email containing your answers. Use your screen reader to work with the email.");
    } else {
        tts::ITextToSpeech::instance().speak("Failed to open an email containing your answers.");
    }
}

void MainWindow::emailFeedback()
{
    email::Emailer emailer;

    if (emailer.openFeedbackEmail()) {
        tts::ITextToSpeech::instance().speak("Opening a feedback email. Use your screen reader to work with the email.");
    } else {
        tts::ITextToSpeech::instance().speak("Failed to open feedback email.");
    }
}

void MainWindow::showFileProperties()
{
    tts::ITextToSpeech::instance().speak(m_crossword.getInformation());
}

void MainWindow::stopSpeech()
{
    tts::ITextToSpeech::instance().speak(" ");
}

void MainWindow::readLastSpokenPhrase()
{
    QString lastSpokenPhrase;

    if (tts::ITextToSpeech::instance().getSpeechLog().isEmpty() == false) {
        lastSpokenPhrase = tts::ITextToSpeech::instance().getSpeechLog().back();
    }

    tts::ITextToSpeech::instance().speak(lastSpokenPhrase);
}

void MainWindow::advanceToNextWordInClue()
{
    tts::ITextToSpeech::instance().speak(m_clueReader->advanceWord());
}

void MainWindow::readCurrentWordInClue()
{
    tts::ITextToSpeech::instance().speak(m_clueReader->getWord());
}

void MainWindow::increaseSpeechRate()
{
    tts::ITextToSpeech::instance().speak(tts::ITextToSpeech::instance().increaseSpeechRate());
}

void MainWindow::decreaseSpeechRate()
{
    tts::ITextToSpeech::instance().speak(tts::ITextToSpeech::instance().decreaseSpeechRate());
}

void MainWindow::scoreCrossword()
{
    tts::ITextToSpeech::instance().speak(m_crossword.getScore());
}

QString MainWindow::getIntroString() const
{
    return QString("Welcome to ").append(version::getApplicationName().append(". "))
           .append("Press ").append(controls::loadShortcutKey).append(" to load a crossword. ")
           .append("Press ").append(controls::exitShortcutKey).append(" to quit the program. ")
           .append("Press ").append(
               controls::helpShortcutKey).append(" to open a help document in your web browser. ")
           .append("Press ").append(
               controls::tutorialShortcutKey).append(" to open a guide document in your web browser. ");
}

void MainWindow::showAbout()
{
    tts::ITextToSpeech::instance().speak(QString(
                                             version::getApplicationName().append(" is a 2D and 3D crossword puzzle game for the blind or partially sighted. ")
                                             .append("You are using ").append(version::getApplicationVersionDescription())));
}

void MainWindow::raiseError(QString title, QString error)
{
    tts::ITextToSpeech::instance().speak(QString(title).append(" - ").append(error));
}

void MainWindow::createShortcuts()
{
    m_loadShortcut = new QShortcut(QKeySequence(controls::loadShortcutKey), this);
    connect(m_loadShortcut, SIGNAL(activated()), this, SLOT(loadCrossword()));

    m_saveShortcut = new QShortcut(QKeySequence(controls::saveShortcutKey), this);
    connect(m_saveShortcut, SIGNAL(activated()), this, SLOT(saveCrossword()));

    m_helpShortcut = new QShortcut(QKeySequence(controls::helpShortcutKey), this);
    connect(m_helpShortcut, SIGNAL(activated()), this, SLOT(openHelp()));

    m_exitShortcut = new QShortcut(QKeySequence(controls::exitShortcutKey), this);
    connect(m_exitShortcut, SIGNAL(activated()), this, SLOT(exitConfirmation()));

    m_tutorialShortcut = new QShortcut(QKeySequence(controls::tutorialShortcutKey), this);
    connect(m_tutorialShortcut, SIGNAL(activated()), this, SLOT(openTutorial()));

    m_emailAnswersShortcut = new QShortcut(QKeySequence(controls::emailAnswersKey), this);
    connect(m_emailAnswersShortcut, SIGNAL(activated()), this, SLOT(emailAnswers()));

    m_emailFeedbackShortcut = new QShortcut(QKeySequence(controls::emailFeedbackKey), this);
    connect(m_emailFeedbackShortcut, SIGNAL(activated()), this, SLOT(emailFeedback()));

    m_printAnswersShortcut = new QShortcut(QKeySequence(controls::printAnswersKey), this);
    connect(m_printAnswersShortcut, SIGNAL(activated()), this, SLOT(printAnswers()));

    m_scoreShortcut = new QShortcut(QKeySequence(controls::markShortcutKey), this);
    connect(m_scoreShortcut, SIGNAL(activated()), this, SLOT(scoreCrossword()));

    m_filePropertiesShortcut = new QShortcut(QKeySequence(controls::filePropertiesShortcutKey),
                                             this);
    connect(m_filePropertiesShortcut, SIGNAL(activated()), this, SLOT(showFileProperties()));

    m_filterTableViewShortcut = new QShortcut(QKeySequence(controls::filterTableViewShortcutKey),
                                              this);
    connect(m_filterTableViewShortcut, SIGNAL(activated()), this, SLOT(cycleTableViewFilter()));

    m_cycleSpeechModeShortcut = new QShortcut(QKeySequence(controls::cycleSpeechModeShortcutKey),
                                              this);
    connect(m_cycleSpeechModeShortcut, SIGNAL(activated()), this, SLOT(cycleSpeechMode()));

    m_stopSpeechShortcut = new QShortcut(QKeySequence(controls::stopSpeechKey), this);
    connect(m_stopSpeechShortcut, SIGNAL(activated()), this, SLOT(stopSpeech()));

    m_readCurrentClueWordShortcut = new QShortcut(QKeySequence(controls::readCurrentClueWordKey),
                                                  this);
    connect(m_readCurrentClueWordShortcut, SIGNAL(activated()), this, SLOT(readCurrentWordInClue()));

    m_advanceCurrentClueWordShortcut = new QShortcut(QKeySequence(controls::advanceClueWordKey),
                                                     this);
    connect(m_advanceCurrentClueWordShortcut, SIGNAL(activated()), this,
            SLOT(advanceToNextWordInClue()));

    m_increaseSpeechRateShortcut = new QShortcut(QKeySequence(controls::increaseSpeechRateKey),
                                                 this);
    connect(m_increaseSpeechRateShortcut, SIGNAL(activated()), this, SLOT(increaseSpeechRate()));

    m_decreaseSpeechRateShortcut = new QShortcut(QKeySequence(controls::decreaseSpeechRateKey),
                                                 this);
    connect(m_decreaseSpeechRateShortcut, SIGNAL(activated()), this, SLOT(decreaseSpeechRate()));

    m_readLastSpokenPhraseShortcut = new QShortcut(QKeySequence(controls::readLastSpokenPhraseKey),
                                                   this);
    connect(m_readLastSpokenPhraseShortcut, SIGNAL(activated()), this, SLOT(readLastSpokenPhrase()));

    m_cycleViewVisibilityShortcut = new QShortcut(QKeySequence(controls::cycleViewVisibilityKey),
                                                  this);
    connect(m_cycleViewVisibilityShortcut, SIGNAL(activated()), this, SLOT(cycleViewVisibility()));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QuitDialog dialog;

    tts::ITextToSpeech::instance().speak(dialog.getBodyString());

    if (dialog.exec()) {
        // Quits silently rather than synchronously reading "goodbye" message
        event->accept();
    } else {
        tts::ITextToSpeech::instance().speak(dialog.getRejectedText());
        event->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    event->accept();
}

void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
    event->accept();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent* event)
{
    if (!event->mimeData()->hasUrls()) {
        return;
    }

    QStringList urls;

    for (QUrl& url : event->mimeData()->urls()) {
        urls << url.toLocalFile();
    }

    if (urls.empty()) {
        return;
    }

    const QString firstFile = urls.first();

    if (loadCrosswordPuzzle(firstFile, m_crossword, m_crosswordLoader)) {
        emit puzzleLoaded();
        QFileInfo fileInfo(firstFile);
        tts::ITextToSpeech::instance().speak(fileInfo.completeBaseName().append(" was loaded."));
    } else {
        tts::ITextToSpeech::instance().speak("Failed to load crossword.");
    }
}

void MainWindow::exitConfirmation()
{
    close();
}

}
