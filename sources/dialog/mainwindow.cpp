#include "dialog/mainwindow.h"

#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QShortcut>
#include <QSortFilterProxyModel>
#include <QUrl>

#include "shortcutkeys.h"
#include "crossword/cluereader.h"
#include "crossword/crosswordbase.h"
#include "dialog/crosswordentrytablemodel.h"
#include "dialog/filedialog.h"
#include "dialog/graphicalgridscene.h"
#include "dialog/quitdialog.h"
#include "email/emailer.h"
#include "printing/printer.h"
#include "tts/itexttospeech.h"
#include "util/util.h"
#include "version/version.h"

// TODO ensure this is created and scanned when loading crosswords in future
const QString MainWindow::DEFAULT_SAVE_FOLDER = QString("/saved_crosswords");

const QString MainWindow::HELP_FILE_LOCATION = QString(":/assets/help/help.html");
const QString MainWindow::TUTORIAL_FILE_LOCATION = QString(":/assets/help/tutorial.html");
const QString MainWindow::LICENSE_FILE_LOCATION = QString(":/assets/license/gplv3.htm");
const QString MainWindow::WEBSITE_ADDRESS_LOCATION =
    QString(":/assets/config/calendarpuzzles_website_address.txt");

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(version::getApplicationName().append(" (Version ").append(
                       version::getApplicationVersion().append(")")));
    setWindowIcon(QIcon(":/icon.ico"));

    createShortcuts();

    m_graphicsScene = new GraphicalGridScene(m_crossword);
    ui->graphicsView->setScene(m_graphicsScene);

    m_tableModel = new CrosswordEntryTableModel(m_crossword, m_crossword.getRefEntries());
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_tableModel);
    ui->wordTableView->setModel(m_proxyModel);

    m_clueReader = new crossword::ClueReader();

    connect(this, SIGNAL(puzzleLoaded()), m_tableModel, SLOT(crosswordEntriesChanged()));
    connect(this, SIGNAL(puzzleLoaded()), ui->wordTableView, SLOT(setFocus(Qt::OtherFocusReason)));
    connect(this, SIGNAL(puzzleLoaded()), m_graphicsScene, SLOT(buildPuzzleGrid()));

    connect(m_tableModel, SIGNAL(conflictingWordError()), ui->wordTableView,
            SLOT(conflictingWordError()));
    connect(ui->wordTableView, SIGNAL(modelIndexChanged(const QModelIndex&, const QModelIndex&)),
            m_tableModel, SLOT(tableViewSelectionChanged(const QModelIndex&, const QModelIndex&)));

    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString, QModelIndex)), this,
            SLOT(checkIfPuzzleWasCompleted()));
    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString, QModelIndex)), m_tableModel,
            SLOT(enterGuess(QString, QModelIndex)));
    connect(ui->wordTableView, SIGNAL(guessAmendationRequested(QString, QModelIndex)), m_tableModel,
            SLOT(amendGuess(QString, QModelIndex)));
    connect(ui->wordTableView, SIGNAL(guessErasureRequested(QModelIndex)), m_tableModel,
            SLOT(eraseGuess(QModelIndex)));

    connect(m_tableModel, SIGNAL(guessValidated(QString)), ui->wordTableView,
            SLOT(reportGuessAccepted(QString)));
    connect(m_tableModel, SIGNAL(guessAmended(QString)), ui->wordTableView,
            SLOT(reportGuessAmended(QString)));
    connect(m_tableModel, SIGNAL(guessErased()), ui->wordTableView, SLOT(reportGuessErased()));
    connect(m_tableModel, SIGNAL(guessAmendationRequestRejected()), ui->wordTableView,
            SLOT(reportGuessAmendationRejected()));

    connect(m_tableModel, SIGNAL(guessValidated(QString)), m_graphicsScene, SLOT(repaintPuzzleGrid()));
    connect(m_tableModel, SIGNAL(guessAmended(QString)), m_graphicsScene, SLOT(repaintPuzzleGrid()));
    connect(m_tableModel, SIGNAL(guessErased()), m_graphicsScene, SLOT(repaintPuzzleGrid()));
    connect(m_tableModel, SIGNAL(crosswordEntrySelectionChanged(CrosswordEntry)), m_graphicsScene,
            SLOT(highlightSelection(CrosswordEntry)));

    connect(&m_crosswordLoader, SIGNAL(loaderError(QString, QString)), this, SLOT(showError(QString,
                                                                                            QString)));

    connect(m_tableModel, SIGNAL(crosswordEntrySelectionChanged(CrosswordEntry)), m_clueReader,
            SLOT(setText(CrosswordEntry)));

    tts::ITextToSpeech::instance().speak(getIntroString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCrossword()
{
    QDir dir;
    QString loadPath;

    if (dir.exists(dir.absolutePath().append(DEFAULT_SAVE_FOLDER))) {
        loadPath = dir.absolutePath().append(DEFAULT_SAVE_FOLDER);
    } else {
        loadPath = dir.absolutePath();
    }

    tts::ITextToSpeech::instance().speak("Opening crossword list. Use the arrow keys to navigate the list.");

    QStringList fileNameFilter;
    fileNameFilter.push_back(QString("*.xwc"));
    fileNameFilter.push_back(QString("*.xwc3d"));
    fileNameFilter.push_back(QString("*.xwcr3d"));

    FileDialog crosswordDialog(this,
                               "Choose a crossword to load",
                               loadPath,
                               "Crossword Grid 3D(*.xwc3d);; Crossword Disc 3D(*.xwcr3d);;Crossword Compiler (*.xwc)",
                               fileNameFilter);

    QString path;

    if (crosswordDialog.exec()) {
        path = crosswordDialog.getSelectedFilePath();
    }

    if (!path.isNull()) {
        QFileInfo fileInfo(path);
        QString extension = fileInfo.suffix();

        if (m_crosswordLoader.loadPuzzle(m_crossword, path, extension)) {
            emit puzzleLoaded();

            tts::ITextToSpeech::instance().speak(fileInfo.completeBaseName().append(" was loaded."));
        }
    } else {
        tts::ITextToSpeech::instance().speak("No crossword was selected.");
    }
}

void MainWindow::saveCrossword()
{
    QDir dir;
    QString path = dir.absolutePath()
                   .append(DEFAULT_SAVE_FOLDER)
                   .append("/")
                   .append(m_crossword.getTitle())
                   .append(".")
                   .append(m_crossword.getFormat());

    QFileInfo fileInfo(path);

    unsigned int extraTag = 1;
    QString separatorTag = QString("_");

    while (dir.exists(path)) {
        QString updatedFileName = fileInfo.baseName()
                                  .append(separatorTag)
                                  .append(QString::number(extraTag))
                                  .append(".")
                                  .append(m_crossword.getFormat());

        path = dir.absolutePath()
               .append(DEFAULT_SAVE_FOLDER)
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
            m_proxyModel->setFilterKeyColumn(CrosswordEntryTableHeader::wordColumnId);
            tts::ITextToSpeech::instance().speak("Showing unstarted crossword entries.");
            break;

        case 1:
            m_proxyModel->setFilterRegExp(showCompleted);
            m_proxyModel->setFilterKeyColumn(CrosswordEntryTableHeader::wordColumnId);
            tts::ITextToSpeech::instance().speak("Showing completed crossword entries.");
            break;

        case 2:
            m_proxyModel->setFilterRegExp(showPartial);
            m_proxyModel->setFilterKeyColumn(CrosswordEntryTableHeader::wordColumnId);
            tts::ITextToSpeech::instance().speak("Showing partially completed crossword entries.");
            break;

        case 3:
            m_proxyModel->setFilterRegExp(showAll);
            m_proxyModel->setFilterKeyColumn(CrosswordEntryTableHeader::wordColumnId);
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
    QDir dir;
    QString filePath = dir.absolutePath().append(LICENSE_FILE_LOCATION);
    QUrl url = QUrl::fromLocalFile(filePath);

    bool openedSuccessfully = false;

    if (dir.exists(filePath)) {
        openedSuccessfully = util::openUrl(url);
    }

    if (openedSuccessfully) {
        tts::ITextToSpeech::instance().speak("Opening license document in web browser. Use your screen reader to read the license document.");
    } else {
        tts::ITextToSpeech::instance().speak("Error, could not open license document.");
    }
}

void MainWindow::openHelp()
{
    QDir dir;
    QString filePath = dir.absolutePath().append(HELP_FILE_LOCATION);
    QUrl url = QUrl::fromLocalFile(filePath);

    bool openedSuccessfully = false;

    if (dir.exists(filePath)) {
        openedSuccessfully = util::openUrl(url);
    }

    if (openedSuccessfully) {
        tts::ITextToSpeech::instance().speak("Opening help page in web browser. Use your screen reader to read the help page.");
    } else {
        tts::ITextToSpeech::instance().speak("Error, could not open help page.");
    }
}

void MainWindow::openTutorial()
{
    QDir dir;
    QString filePath = dir.absolutePath().append(TUTORIAL_FILE_LOCATION);
    QUrl url = QUrl::fromLocalFile(filePath);

    bool openedSuccessfully = false;

    if (dir.exists(filePath)) {
        openedSuccessfully = util::openUrl(url);
    }

    if (openedSuccessfully) {
        tts::ITextToSpeech::instance().speak("Opening guide page in web browser. Use your screen reader to read the guide page.");
    } else {
        tts::ITextToSpeech::instance().speak("Error, could not open guide page.");
    }
}

void MainWindow::openCalendarPuzzlesWebsite()
{
    QDir dir;
    QString filePath = dir.absolutePath().append(WEBSITE_ADDRESS_LOCATION);

    if (util::fileExists(filePath)) {
        QStringList address;
        util::readFile(address, filePath);

        if (!address.empty()) {
            if (util::openUrl(QUrl(address.takeFirst()))) {
                tts::ITextToSpeech::instance().speak("Opening Calendar Puzzles website in web browser. Use your screen reader to read the website.");
            } else {
                tts::ITextToSpeech::instance().speak("Error, could not open Calendar Puzzles website.");
            }
        }
    } else {
        tts::ITextToSpeech::instance().speak("Error, could not find Calendar Puzzles website address.");
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
    tts::ITextToSpeech::instance().speak("");
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
           .append("Press ").append(ShortcutKeys::loadShortcutKey).append(" to load a crossword. ")
           .append("Press ").append(ShortcutKeys::exitShortcutKey).append(" to quit the program. ")
           .append("Press ").append(
               ShortcutKeys::helpShortcutKey).append(" to open a help document in your web browser. ")
           .append("Press ").append(
               ShortcutKeys::tutorialShortcutKey).append(" to open a guide document in your web browser. ");
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
    m_loadShortcut = new QShortcut(QKeySequence(ShortcutKeys::loadShortcutKey), this);
    connect(m_loadShortcut, SIGNAL(activated()), this, SLOT(loadCrossword()));

    m_saveShortcut = new QShortcut(QKeySequence(ShortcutKeys::saveShortcutKey), this);
    connect(m_saveShortcut, SIGNAL(activated()), this, SLOT(saveCrossword()));

    m_helpShortcut = new QShortcut(QKeySequence(ShortcutKeys::helpShortcutKey), this);
    connect(m_helpShortcut, SIGNAL(activated()), this, SLOT(openHelp()));

    m_exitShortcut = new QShortcut(QKeySequence(ShortcutKeys::exitShortcutKey), this);
    connect(m_exitShortcut, SIGNAL(activated()), this, SLOT(exitConfirmation()));

    m_tutorialShortcut = new QShortcut(QKeySequence(ShortcutKeys::tutorialShortcutKey), this);
    connect(m_tutorialShortcut, SIGNAL(activated()), this, SLOT(openTutorial()));

    m_emailAnswersShortcut = new QShortcut(QKeySequence(ShortcutKeys::emailAnswersKey), this);
    connect(m_emailAnswersShortcut, SIGNAL(activated()), this, SLOT(emailAnswers()));

    m_emailFeedbackShortcut = new QShortcut(QKeySequence(ShortcutKeys::emailFeedbackKey), this);
    connect(m_emailFeedbackShortcut, SIGNAL(activated()), this, SLOT(emailFeedback()));

    m_printAnswersShortcut = new QShortcut(QKeySequence(ShortcutKeys::printAnswersKey), this);
    connect(m_printAnswersShortcut, SIGNAL(activated()), this, SLOT(printAnswers()));

    m_scoreShortcut = new QShortcut(QKeySequence(ShortcutKeys::markShortcutKey), this);
    connect(m_scoreShortcut, SIGNAL(activated()), this, SLOT(scoreCrossword()));

    m_filePropertiesShortcut = new QShortcut(QKeySequence(ShortcutKeys::filePropertiesShortcutKey),
                                             this);
    connect(m_filePropertiesShortcut, SIGNAL(activated()), this, SLOT(showFileProperties()));

    m_filterTableViewShortcut = new QShortcut(QKeySequence(ShortcutKeys::filterTableViewShortcutKey),
                                              this);
    connect(m_filterTableViewShortcut, SIGNAL(activated()), this, SLOT(cycleTableViewFilter()));

    m_cycleSpeechModeShortcut = new QShortcut(QKeySequence(ShortcutKeys::cycleSpeechModeShortcutKey),
                                              this);
    connect(m_cycleSpeechModeShortcut, SIGNAL(activated()), this, SLOT(cycleSpeechMode()));

    m_stopSpeechShortcut = new QShortcut(QKeySequence(ShortcutKeys::stopSpeechKey), this);
    connect(m_stopSpeechShortcut, SIGNAL(activated()), this, SLOT(stopSpeech()));

    m_readCurrentClueWordShortcut = new QShortcut(QKeySequence(ShortcutKeys::readCurrentClueWordKey),
                                                  this);
    connect(m_readCurrentClueWordShortcut, SIGNAL(activated()), this, SLOT(readCurrentWordInClue()));

    m_advanceCurrentClueWordShortcut = new QShortcut(QKeySequence(ShortcutKeys::advanceClueWordKey),
                                                     this);
    connect(m_advanceCurrentClueWordShortcut, SIGNAL(activated()), this,
            SLOT(advanceToNextWordInClue()));

    m_increaseSpeechRateShortcut = new QShortcut(QKeySequence(ShortcutKeys::increaseSpeechRateKey),
                                                 this);
    connect(m_increaseSpeechRateShortcut, SIGNAL(activated()), this, SLOT(increaseSpeechRate()));

    m_decreaseSpeechRateShortcut = new QShortcut(QKeySequence(ShortcutKeys::decreaseSpeechRateKey),
                                                 this);
    connect(m_decreaseSpeechRateShortcut, SIGNAL(activated()), this, SLOT(decreaseSpeechRate()));

    m_readLastSpokenPhraseShortcut = new QShortcut(QKeySequence(ShortcutKeys::readLastSpokenPhraseKey),
                                                   this);
    connect(m_readLastSpokenPhraseShortcut, SIGNAL(activated()), this, SLOT(readLastSpokenPhrase()));

    m_cycleViewVisibilityShortcut = new QShortcut(QKeySequence(ShortcutKeys::cycleViewVisibilityKey),
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

void MainWindow::exitConfirmation()
{
    close();
}
