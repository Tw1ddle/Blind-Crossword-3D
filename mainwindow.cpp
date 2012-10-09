#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QSortFilterProxyModel>

#include "puzzlebase.h"
#include "gridpuzzle.h"
#include "discpuzzle.h"

#include "grid3dgraphicsscene.h"
#include "wordtablemodel.h"
#include "itexttospeech.h"
#include "quitdialog.h"
#include "filedialog.h"
#include "shortcutkeys.h"

const QString MainWindow::m_DefaultSaveFolder = QString("/Crosswords");
const QString MainWindow::m_HelpFileLocation = QString("/Help/index.html");
const QString MainWindow::m_LicenseFileLocation = QString("/License/gplv3.htm");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon.ico"));

    createShortcuts();

    m_Puzzle = new GridPuzzle();

    m_GraphicsScene = new Grid3DGraphicsScene(m_Puzzle);
    ui->graphicsView->setScene(m_GraphicsScene);

    m_WordTableModel = new WordTableModel(m_Puzzle);
    m_ProxyModel = new QSortFilterProxyModel(this);
    m_ProxyModel->setSourceModel(m_WordTableModel);
    ui->wordTableView->setModel(m_ProxyModel);

    connect(this, SIGNAL(puzzleLoaded()), m_WordTableModel, SLOT(crosswordEntriesChanged()));
    connect(this, SIGNAL(puzzleLoaded()), ui->wordTableView, SLOT(setFocus(Qt::OtherFocusReason)));
    connect(this, SIGNAL(puzzleLoaded()), m_GraphicsScene, SLOT(buildPuzzleGrid()));

    connect(m_WordTableModel, SIGNAL(conflictingWordError()), ui->wordTableView, SLOT(conflictingWordError()));

    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString,QModelIndex)), this, SLOT(checkIfPuzzleWasCompleted()));
    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString, QModelIndex)), m_WordTableModel, SLOT(enterGuess(QString, QModelIndex)));
    connect(ui->wordTableView, SIGNAL(guessAmendationRequested(QString, QModelIndex)), m_WordTableModel, SLOT(amendGuess(QString, QModelIndex)));
    connect(ui->wordTableView, SIGNAL(modelIndexChanged(const QModelIndex&, const QModelIndex&)), m_WordTableModel, SLOT(tableViewSelectionChanged(const QModelIndex&, const QModelIndex&)));

    connect(m_WordTableModel, SIGNAL(guessValidated(QString)), ui->wordTableView, SLOT(reportGuessAccepted(QString)));
    connect(m_WordTableModel, SIGNAL(guessAmended(QString)), ui->wordTableView, SLOT(reportGuessAmended(QString)));

    connect(m_WordTableModel, SIGNAL(guessValidated(QString)), m_GraphicsScene, SLOT(repaintPuzzleGrid()));
    connect(m_WordTableModel, SIGNAL(guessAmended(QString)), m_GraphicsScene, SLOT(repaintPuzzleGrid()));
    connect(m_WordTableModel, SIGNAL(crosswordEntrySelectionChanged(CrosswordEntry3D)), m_GraphicsScene, SLOT(highlightSelection(CrosswordEntry3D)));

    connect(&m_PuzzleLoader, SIGNAL(loaderError(QString, QString)), this, SLOT(showError(QString, QString)));

    ITextToSpeech::instance().speak(getIntroString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QuitDialog dialog;

    ITextToSpeech::instance().speak(dialog.getBodyString());

    if(dialog.exec())
    {
        ITextToSpeech::instance().speak(dialog.getAcceptedText(), csDefaultSynchronousSpeechOptions);
        event->accept();
    }
    else
    {
        ITextToSpeech::instance().speak(dialog.getRejectedText());
        event->ignore();
    }
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

    ITextToSpeech::instance().speak(tr("Opening load crossword dialog."));

    QStringList fileNameFilter;
    fileNameFilter.push_back(QString("*.xwc"));
    fileNameFilter.push_back(QString("*.xwc3d"));
    fileNameFilter.push_back(QString("*.xwcr3d"));

    FileDialog crosswordDialog(this,
                               tr("Choose a crossword to load"),
                               loadPath,
                               tr("Crossword Grid 3D(*.xwc3d);; Crossword Disc 3D(*.xwcr3d);;Crossword Compiler (*.xwc)"),
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

        if(m_PuzzleLoader.loadPuzzle(*m_Puzzle, path, extension))
        {
            emit puzzleLoaded();
            ITextToSpeech::instance().speak(fileInfo.fileName().append(" was loaded successfully."));
        }
    }
    else
    {
        ITextToSpeech::instance().speak(tr("No file was selected."));
    }
}

void MainWindow::saveCrossword()
{
    QDir dir;
    QString path = dir.absolutePath()
            .append(m_DefaultSaveFolder)
            .append("/")
            .append(m_Puzzle->getPuzzleTitle())
            .append(".")
            .append(m_Puzzle->getPuzzleFormat());

    QFileInfo fileInfo(path);

    unsigned int extraTag = 1;
    QString separatorTag = QString("_");
    while(dir.exists(path))
    {
        QString updatedFileName = fileInfo.baseName()
                .append(separatorTag)
                .append(QString::number(extraTag))
                .append(".")
                .append(m_Puzzle->getPuzzleFormat());

        path = dir.absolutePath()
                .append(m_DefaultSaveFolder)
                .append("/")
                .append(updatedFileName);

        extraTag++;
    }

    QFileInfo updatedFileInfo(path);
    if(m_PuzzleLoader.savePuzzle(*m_Puzzle, path, m_Puzzle->getPuzzleFormat()))
    {
        ITextToSpeech::instance().speak(tr("Crossword was saved as: ")
                                        .append(updatedFileInfo.fileName())
                                        .append(". In folder: ")
                                        .append(updatedFileInfo.filePath()));
    }
}

void MainWindow::showFileProperties()
{
    ITextToSpeech::instance().speak(m_Puzzle->getInformationString());
}

void MainWindow::showFileThemePhrase()
{
    ITextToSpeech::instance().speak(m_Puzzle->getPuzzleThemePhrase());
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
        ITextToSpeech::instance().speak(tr("Opening license document in web browser."));
    }
    else
    {
        ITextToSpeech::instance().speak(tr("Error, could not open license document."));
    }
}

void MainWindow::toggleGrid(bool hidden)
{
    if(!hidden)
    {
        ITextToSpeech::instance().speak(tr("Crossword grid is now hidden."));
        ui->graphicsView->setVisible(hidden);
    }
    else
    {
        ITextToSpeech::instance().speak(tr("Crossword grid is now shown."));
        ui->graphicsView->setVisible(hidden);
    }
}

void MainWindow::readCrosswordThemePhrase()
{
    ITextToSpeech::instance().speak(m_Puzzle->getPuzzleThemePhrase());
}

void MainWindow::cycleSpeechMode()
{
    const static unsigned int cs_NumSpeechModes = 2;
    static unsigned int s_SpeechMode = 0;

    switch(s_SpeechMode)
    {
        case 0:
        ITextToSpeech::instance().speak(tr("Spelling speech mode activated."));
        ITextToSpeech::instance().setMode(SPEECH_MODES::spellingOutSpeech);
        break;
        case 1:
        ITextToSpeech::instance().setMode(SPEECH_MODES::normalSpeech);
        ITextToSpeech::instance().speak(tr("Regular speech mode activated."));
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
    const static unsigned int cs_NumFilters = 5;
    static unsigned int s_Filter = 0;
    QRegExp filterUnstarted = QRegExp("[^a-zA-Z]");
    QRegExp filterPartial = QRegExp("([^\\.])");
    QRegExp filterCompleted = QRegExp("");
    QRegExp filterNone = QRegExp("");
    QRegExp filterIncidentCrosswordEntries = QRegExp("");

    switch(s_Filter)
    {
        case 0:
            m_ProxyModel->setFilterRegExp(filterUnstarted);
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak(tr("Showing only unstarted crossword entries."));
            break;
        case 1:
            m_ProxyModel->setFilterRegExp(filterCompleted);
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak(tr("Showing only completed crossword entries."));
            break;
        case 2:
            m_ProxyModel->setFilterRegExp(filterPartial);
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak(tr("Showing only partially completed crossword entries."));
            break;
        case 3:
             m_ProxyModel->setFilterRegExp(filterNone);
             m_ProxyModel->setFilterKeyColumn(1);
             ITextToSpeech::instance().speak(tr("Filtering disabled."));
            break;
       case 4:
            m_ProxyModel->setFilterKeyColumn(0);
    }

    s_Filter++;
    if(s_Filter >= cs_NumFilters)
    {
        s_Filter = 0;
    }
}

void MainWindow::checkIfPuzzleWasCompleted()
{
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
        ITextToSpeech::instance().speak(tr("Opening help page in web browser."));
    }
    else
    {
        ITextToSpeech::instance().speak(tr("Error, could not open help page."));
    }
}

void MainWindow::scoreCrossword()
{
    ITextToSpeech::instance().speak(m_Puzzle->getScoreString());
}

void MainWindow::raiseError(QString title, QString error)
{
    ITextToSpeech::instance().speak(QString(title).append(" - ").append(error));
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
}

QString MainWindow::getIntroString() const
{
    return QString(tr("Welcome to Blind Crossword 3D. Press ").append(ShortcutKeys::loadShortcutKey).append(" to load a crossword. "))
            .append("Press ").append(ShortcutKeys::exitShortcutKey).append(" to quit the program. ")
            .append("Press ").append(ShortcutKeys::helpShortcutKey).append(" to open a help document in your web browser. Use your screen reader to read the document");
}

void MainWindow::showAbout()
{
    ITextToSpeech::instance().speak(tr("Blind Crossword3D is a 2D and 3D crossword puzzle game for the blind or partially sighted."));
}

void MainWindow::exitConfirmation()
{
    close();
}
