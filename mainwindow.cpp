#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QSortFilterProxyModel>

#include "version.h"

#include "puzzlebase.h"
#include "grid3dgraphicsscene.h"
#include "wordtablemodel.h"

#include "shortcutkeys.h"
#include "quitdialog.h"
#include "filedialog.h"
#include "itexttospeech.h"
#include "idlereminder.h"

#include "advancedcluereader.h"
#include "emailer.h"
#include "printer.h"

const QString MainWindow::m_DefaultSaveFolder = QString("/Crosswords");
const QString MainWindow::m_HelpFileLocation = QString("/Help/index.html");
const QString MainWindow::m_LicenseFileLocation = QString("/License/gplv3.htm");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_ApplicationOpenReminderEnabled(true),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(Version::getApplicationName());
    setWindowIcon(QIcon(":/icon.ico"));

    createShortcuts();

    m_GraphicsScene = new Grid3DGraphicsScene(m_Puzzle);
    ui->graphicsView->setScene(m_GraphicsScene);

    m_WordTableModel = new WordTableModel(m_Puzzle, m_Puzzle.getRefCrosswordEntries());
    m_ProxyModel = new QSortFilterProxyModel(this);
    m_ProxyModel->setSourceModel(m_WordTableModel);
    ui->wordTableView->setModel(m_ProxyModel);

    m_IdleReminder = new IdleReminder(60000);
    m_AdvancedClueReader = new ClueReader();

    connect(this, SIGNAL(puzzleLoaded()), m_WordTableModel, SLOT(crosswordEntriesChanged()));
    connect(this, SIGNAL(puzzleLoaded()), ui->wordTableView, SLOT(setFocus(Qt::OtherFocusReason)));
    connect(this, SIGNAL(puzzleLoaded()), m_GraphicsScene, SLOT(buildPuzzleGrid()));

    connect(m_WordTableModel, SIGNAL(conflictingWordError()), ui->wordTableView, SLOT(conflictingWordError()));
    connect(ui->wordTableView, SIGNAL(modelIndexChanged(const QModelIndex&, const QModelIndex&)), m_WordTableModel, SLOT(tableViewSelectionChanged(const QModelIndex&, const QModelIndex&)));

    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString,QModelIndex)), this, SLOT(checkIfPuzzleWasCompleted()));
    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString, QModelIndex)), m_WordTableModel, SLOT(enterGuess(QString, QModelIndex)));
    connect(ui->wordTableView, SIGNAL(guessAmendationRequested(QString, QModelIndex)), m_WordTableModel, SLOT(amendGuess(QString, QModelIndex)));
    connect(ui->wordTableView, SIGNAL(guessErasureRequested(QModelIndex)), m_WordTableModel, SLOT(eraseGuess(QModelIndex)));

    connect(m_WordTableModel, SIGNAL(guessValidated(QString)), ui->wordTableView, SLOT(reportGuessAccepted(QString)));
    connect(m_WordTableModel, SIGNAL(guessAmended(QString)), ui->wordTableView, SLOT(reportGuessAmended(QString)));
    connect(m_WordTableModel, SIGNAL(guessErased()), ui->wordTableView, SLOT(reportGuessErased()));
    connect(m_WordTableModel, SIGNAL(guessAmendationRequestRejected()), ui->wordTableView, SLOT(reportGuessAmendationRejected()));

    connect(m_WordTableModel, SIGNAL(guessValidated(QString)), m_GraphicsScene, SLOT(repaintPuzzleGrid()));
    connect(m_WordTableModel, SIGNAL(guessAmended(QString)), m_GraphicsScene, SLOT(repaintPuzzleGrid()));
    connect(m_WordTableModel, SIGNAL(guessErased()), m_GraphicsScene, SLOT(repaintPuzzleGrid()));
    connect(m_WordTableModel, SIGNAL(crosswordEntrySelectionChanged(CrosswordEntry3D)), m_GraphicsScene, SLOT(highlightSelection(CrosswordEntry3D)));

    connect(&m_PuzzleLoader, SIGNAL(loaderError(QString, QString)), this, SLOT(showError(QString, QString)));

    connect(m_IdleReminder, SIGNAL(timedOut()), this, SLOT(onIdleReminderTimeout()));
    qApp->installEventFilter(m_IdleReminder);

    connect(m_WordTableModel, SIGNAL(crosswordEntrySelectionChanged(CrosswordEntry3D)), m_AdvancedClueReader, SLOT(setText(CrosswordEntry3D)));

    ITextToSpeech::instance().speak(getIntroString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCrossword()
{
    QDir dir;
    QString loadPath;
    if(dir.exists(dir.absolutePath().append(m_DefaultSaveFolder)))
    {
        loadPath = dir.absolutePath().append(m_DefaultSaveFolder);
    }
    else
    {
        loadPath = dir.absolutePath();
    }

    ITextToSpeech::instance().speak("Opening load crossword dialog.");

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
    if(crosswordDialog.exec())
    {
         path = crosswordDialog.getSelectedFilePath();
    }
    if(!path.isNull())
    {
        QFileInfo fileInfo(path);
        QString extension = fileInfo.suffix();

        if(m_PuzzleLoader.loadPuzzle(m_Puzzle, path, extension))
        {
            emit puzzleLoaded();

            ITextToSpeech::instance().speak(fileInfo.fileName().append(" was loaded successfully."));
        }
    }
    else
    {
        ITextToSpeech::instance().speak("No file was selected.");
    }
}

void MainWindow::saveCrossword()
{
    QDir dir;
    QString path = dir.absolutePath()
            .append(m_DefaultSaveFolder)
            .append("/")
            .append(m_Puzzle.getPuzzleTitle())
            .append(".")
            .append(m_Puzzle.getPuzzleFormat());

    QFileInfo fileInfo(path);

    unsigned int extraTag = 1;
    QString separatorTag = QString("_");
    while(dir.exists(path))
    {
        QString updatedFileName = fileInfo.baseName()
                .append(separatorTag)
                .append(QString::number(extraTag))
                .append(".")
                .append(m_Puzzle.getPuzzleFormat());

        path = dir.absolutePath()
                .append(m_DefaultSaveFolder)
                .append("/")
                .append(updatedFileName);

        extraTag++;
    }

    QFileInfo updatedFileInfo(path);
    if(m_PuzzleLoader.savePuzzle(m_Puzzle, path, m_Puzzle.getPuzzleFormat()))
    {
        ITextToSpeech::instance().speak(QString("Crossword was saved as: ")
                                        .append(updatedFileInfo.fileName())
                                        .append(". In folder: ")
                                        .append(updatedFileInfo.filePath()));
    }
}

void MainWindow::viewLicense()
{
    QDir dir;
    QString filePath = dir.absolutePath().append(m_LicenseFileLocation);
    QUrl url = QUrl::fromLocalFile(filePath);

    bool openedSuccessfully = false;
    if(dir.exists(filePath))
    {
        openedSuccessfully = QDesktopServices::openUrl(url);
    }

    if(openedSuccessfully)
    {
        ITextToSpeech::instance().speak("Opening license document in web browser. Use your screen reader to read the license document.");
    }
    else
    {
        ITextToSpeech::instance().speak("Error, could not open license document.");
    }
}

void MainWindow::cycleSpeechMode()
{
    const static unsigned int cs_NumSpeechModes = 2;
    static unsigned int s_SpeechMode = 0;

    switch(s_SpeechMode)
    {
        case 0:
        ITextToSpeech::instance().speak("Spelling speech mode activated. Words will be spelled out letter by letter.");
        ITextToSpeech::instance().setMode(SPEECH_MODES::spellingOutSpeech);
        break;
        case 1:
        ITextToSpeech::instance().setMode(SPEECH_MODES::normalSpeech);
        ITextToSpeech::instance().speak("Regular speech mode activated. Words will be read normally.");
        break;
    }

    s_SpeechMode++;
    if(s_SpeechMode >= cs_NumSpeechModes)
    {
        s_SpeechMode = 0;
    }
}

void MainWindow::cycleTableViewFilter()
{
    const static unsigned int cs_NumFilters = 4;
    static unsigned int s_Filter = 0;
    QRegExp showUnstarted = QRegExp("[^A]");
    QRegExp showPartial = QRegExp("([^\\.])");
    QRegExp showCompleted = QRegExp("");
    QRegExp showAll = QRegExp("");
    QRegExp showIncidentCrosswordEntries = QRegExp("");

    switch(s_Filter)
    {
        case 0:
            m_ProxyModel->setFilterRegExp(showUnstarted);
            m_ProxyModel->setFilterKeyColumn(2);
            ITextToSpeech::instance().speak("Showing unstarted crossword entries.");
            break;
        case 1:
            m_ProxyModel->setFilterRegExp(showCompleted);
            m_ProxyModel->setFilterKeyColumn(2);
            ITextToSpeech::instance().speak("Showing completed crossword entries.");
            break;
        case 2:
            m_ProxyModel->setFilterRegExp(showPartial);
            m_ProxyModel->setFilterKeyColumn(2);
            ITextToSpeech::instance().speak("Showing partially completed crossword entries.");
            break;
        case 3:
             m_ProxyModel->setFilterRegExp(showAll);
             m_ProxyModel->setFilterKeyColumn(2);
             ITextToSpeech::instance().speak("Filtering disabled.");
            break;
       case 4:
            m_ProxyModel->setFilterRegExp(showIncidentCrosswordEntries);
            m_ProxyModel->setFilterKeyColumn(2);
            ITextToSpeech::instance().speak("Showing all clues that intersect the current clue");
            break;
    }

    s_Filter++;
    if(s_Filter >= cs_NumFilters)
    {
        s_Filter = 0;
    }
}

void MainWindow::openHelp()
{
    QDir dir;
    QString filePath = dir.absolutePath().append(m_HelpFileLocation);
    QUrl url = QUrl::fromLocalFile(filePath);

    bool openedSuccessfully = false;
    if(dir.exists(filePath))
    {
        openedSuccessfully = QDesktopServices::openUrl(url);
    }

    if(openedSuccessfully)
    {
        ITextToSpeech::instance().speak("Opening help page in web browser. Use your screen reader to read the help page.");
    }
    else
    {
        ITextToSpeech::instance().speak("Error, could not open help page.");
    }
}

void MainWindow::printCrossword()
{
    ITextToSpeech::instance().speak("Attempting to open a print dialog for printing your answers. Use your screen reader to work with the dialog.");

    Printer printer;

    QString result = printer.openPrintDialog(m_Puzzle, this);

    ITextToSpeech::instance().speak(result);
}

void MainWindow::emailCrossword()
{
    Emailer emailer;

    if(emailer.openSendResultsEmail(m_Puzzle))
    {
        ITextToSpeech::instance().speak("Attempting to open an email containing your answers. Use your screen reader to work with the email.");
    }
    else
    {
        ITextToSpeech::instance().speak("Failed to open an email containing your answers.");
    }
}

void MainWindow::emailFeedback()
{
    Emailer emailer;

    if(emailer.openFeedbackEmail())
    {
        ITextToSpeech::instance().speak("Attempting to open an email for you to send us feedback. Use your screen reader to work with the email.");
    }
    else
    {
        ITextToSpeech::instance().speak("Failed to open an email for you to send us feedback.");
    }
}

void MainWindow::showFileProperties()
{
    ITextToSpeech::instance().speak(m_Puzzle.getInformationString());
}

void MainWindow::toggleGrid(bool hidden)
{
    if(!hidden)
    {
        ITextToSpeech::instance().speak("Graphical crossword grid is now hidden.");
        ui->graphicsView->setVisible(hidden);
    }
    else
    {
        ITextToSpeech::instance().speak("Graphical crossword grid is now shown.");
        ui->graphicsView->setVisible(hidden);
    }
}

void MainWindow::toggleApplicationOpenReminder()
{
    if(m_ApplicationOpenReminderEnabled)
    {
        ITextToSpeech::instance().speak("Reminder disabled");

        m_ApplicationOpenReminderEnabled = false;
    }
    else
    {
        ITextToSpeech::instance().speak("Reminder enabled");

        m_ApplicationOpenReminderEnabled = true;
    }
}

void MainWindow::onIdleReminderTimeout()
{
    if(m_ApplicationOpenReminderEnabled)
    {
        ITextToSpeech::instance().speak(QString(Version::getApplicationName().append(" is still running. You can press ").append(ShortcutKeys::helpShortcutKey).append( "to open a help page. To toggle this reminder, press ").append(ShortcutKeys::toggleApplicationOpenReminderKey).append(".")));
    }
}

void MainWindow::readCrosswordThemePhrase()
{
    ITextToSpeech::instance().speak(m_Puzzle.getPuzzleThemePhrase());
}

void MainWindow::stopSpeech()
{
    ITextToSpeech::instance().speak("", SPEECH_MODES::csDefaultAsynchronousSpeechOptions);
}

void MainWindow::advanceToNextWordInClue()
{
    ITextToSpeech::instance().speak(m_AdvancedClueReader->advanceWord());
}

void MainWindow::readCurrentWordInClue()
{
    ITextToSpeech::instance().speak(m_AdvancedClueReader->getWord());
}

void MainWindow::increaseSpeechRate()
{
    ITextToSpeech::instance().speak(ITextToSpeech::instance().increaseSpeechRate());
}

void MainWindow::decreaseSpeechRate()
{
    ITextToSpeech::instance().speak(ITextToSpeech::instance().decreaseSpeechRate());
}

void MainWindow::scoreCrossword()
{
    ITextToSpeech::instance().speak(m_Puzzle.getScoreString());
}

void MainWindow::raiseError(QString title, QString error)
{
    ITextToSpeech::instance().speak(QString(title).append(" - ").append(error));
}

QString MainWindow::getIntroString() const
{
    return QString("Welcome to ").append(Version::getApplicationName().append(". "))
            .append("Press ").append(ShortcutKeys::loadShortcutKey).append(" to load a crossword. ")
            .append("Press ").append(ShortcutKeys::exitShortcutKey).append(" to quit the program. ")
            .append("Press ").append(ShortcutKeys::helpShortcutKey).append(" to open a help document in your web browser. Use your screen reader to read the document");
}

void MainWindow::showAbout()
{
    ITextToSpeech::instance().speak(QString(Version::getApplicationName().append(" is a 2D and 3D crossword puzzle game for the blind or partially sighted. ")
                                            .append("You are using ").append(Version::getApplicationVersionDescription())));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QuitDialog dialog;

    ITextToSpeech::instance().speak(dialog.getBodyString());

    if(dialog.exec())
    {
        ITextToSpeech::instance().speak(dialog.getAcceptedText(), SPEECH_MODES::csDefaultSynchronousSpeechOptions);
        event->accept();
    }
    else
    {
        ITextToSpeech::instance().speak(dialog.getRejectedText());
        event->ignore();
    }
}

void MainWindow::createShortcuts()
{
    m_LoadShortcut = new QShortcut(QKeySequence(ShortcutKeys::loadShortcutKey), this);
    connect(m_LoadShortcut, SIGNAL(activated()), this, SLOT(loadCrossword()));

    m_SaveShortcut = new QShortcut(QKeySequence(ShortcutKeys::saveShortcutKey), this);
    connect(m_SaveShortcut, SIGNAL(activated()), this, SLOT(saveCrossword()));

    m_HelpShortcut = new QShortcut(QKeySequence(ShortcutKeys::helpShortcutKey), this);
    connect(m_HelpShortcut, SIGNAL(activated()), this, SLOT(openHelp()));

    m_ExitShortcut = new QShortcut(QKeySequence(ShortcutKeys::exitShortcutKey), this);
    connect(m_ExitShortcut, SIGNAL(activated()), this, SLOT(exitConfirmation()));

    m_ScoreShortcut = new QShortcut(QKeySequence(ShortcutKeys::scoreShortcutKey), this);
    connect(m_ScoreShortcut, SIGNAL(activated()), this, SLOT(scoreCrossword()));

    m_FilePropertiesShortcut = new QShortcut(QKeySequence(ShortcutKeys::filePropertiesShortcutKey), this);
    connect(m_FilePropertiesShortcut, SIGNAL(activated()), this, SLOT(showFileProperties()));

    m_FilterTableViewShortcut = new QShortcut(QKeySequence(ShortcutKeys::filterTableViewShortcutKey), this);
    connect(m_FilterTableViewShortcut, SIGNAL(activated()), this, SLOT(cycleTableViewFilter()));

    m_CycleSpeechModeShortcut = new QShortcut(QKeySequence(ShortcutKeys::cycleSpeechModeShortcutKey), this);
    connect(m_CycleSpeechModeShortcut, SIGNAL(activated()), this, SLOT(cycleSpeechMode()));

    m_ReadCrosswordThemePhraseShortcut = new QShortcut(QKeySequence(ShortcutKeys::readCrosswordThemePhraseKey), this);
    connect(m_ReadCrosswordThemePhraseShortcut, SIGNAL(activated()), this, SLOT(readCrosswordThemePhrase()));

    m_StopSpeechShortcut = new QShortcut(QKeySequence(ShortcutKeys::stopSpeechKey), this);
    connect(m_StopSpeechShortcut, SIGNAL(activated()), this, SLOT(stopSpeech()));

    m_ApplicationOpenReminderShortcut = new QShortcut(QKeySequence(ShortcutKeys::toggleApplicationOpenReminderKey), this);
    connect(m_ApplicationOpenReminderShortcut, SIGNAL(activated()), this, SLOT(toggleApplicationOpenReminder()));

    m_ReadCurrentClueWordShortcut = new QShortcut(QKeySequence(ShortcutKeys::readCurrentClueWordKey), this);
    connect(m_ReadCurrentClueWordShortcut, SIGNAL(activated()), this, SLOT(readCurrentWordInClue()));

    m_AdvanceCurrentClueWordShortcut = new QShortcut(QKeySequence(ShortcutKeys::advanceClueWordKey), this);
    connect(m_AdvanceCurrentClueWordShortcut, SIGNAL(activated()), this, SLOT(advanceToNextWordInClue()));

    m_IncreaseSpeechRateShortcut = new QShortcut(QKeySequence(ShortcutKeys::increaseSpeechRateKey), this);
    connect(m_IncreaseSpeechRateShortcut, SIGNAL(activated()), this, SLOT(increaseSpeechRate()));

    m_DecreaseSpeechRateShortcut = new QShortcut(QKeySequence(ShortcutKeys::decreaseSpeechRateKey), this);
    connect(m_DecreaseSpeechRateShortcut, SIGNAL(activated()), this, SLOT(decreaseSpeechRate()));
}

void MainWindow::exitConfirmation()
{
    close();
}
