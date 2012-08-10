#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QSortFilterProxyModel>

#include "puzzle3d.h"
#include "grid3dgraphicsscene.h"
#include "wordtablemodel.h"
#include "itexttospeech.h"
#include "quitdialog.h"

const QString MainWindow::m_DefaultSaveFolder = QString("/Crosswords");
const QString MainWindow::m_HelpFileLocation = QString("/Help/index.html");
const QString MainWindow::m_LicenseFileLocation = QString("/License/gplv3.htm");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createShortcuts();

    m_Puzzle = new BCrossword3D();

    m_GraphicsScene = new Grid3DGraphicsScene(&m_Puzzle->getRefGrid(), &m_Puzzle->getRefCrosswordEntries());
    ui->graphicsView->setScene(m_GraphicsScene);

    m_WordTableModel = new WordTableModel(&m_Puzzle->getRefCrosswordEntries(), &m_Puzzle->getRefGrid());
    m_ProxyModel = new QSortFilterProxyModel(this);
    m_ProxyModel->setSourceModel(m_WordTableModel);
    ui->wordTableView->setModel(m_ProxyModel);


    connect(this, SIGNAL(puzzleLoaded()), m_WordTableModel, SLOT(crosswordEntriesChanged()));
    connect(this, SIGNAL(puzzleLoaded()), ui->wordTableView, SLOT(setFocus(Qt::OtherFocusReason)));
    connect(this, SIGNAL(puzzleLoaded()), m_GraphicsScene, SLOT(buildPuzzleGrid()));

    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString, QModelIndex)), m_WordTableModel, SLOT(enterGuess(QString, QModelIndex)));

    connect(m_WordTableModel, SIGNAL(conflictingWordError()), ui->wordTableView, SLOT(conflictingWordError()));
    connect(m_WordTableModel, SIGNAL(guessValidated(QString)), m_GraphicsScene, SLOT(buildPuzzleGrid()));
    connect(m_WordTableModel, SIGNAL(guessAmended(QString)), m_GraphicsScene, SLOT(buildPuzzleGrid()));
    connect(m_WordTableModel, SIGNAL(guessValidated(QString)), ui->wordTableView, SLOT(reportGuessAccepted(QString)));
    connect(ui->wordTableView, SIGNAL(guessAmendationRequested(QModelIndex)), m_WordTableModel, SLOT(amendGuess(QModelIndex)));
    connect(m_WordTableModel, SIGNAL(guessAmended(QString)), ui->wordTableView, SLOT(reportGuessAmended(QString)));

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
        ITextToSpeech::instance().speak(dialog.getAcceptedText(), csSynchronousSpeechOptions);
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
    ITextToSpeech::instance().speak(tr("Opening load crossword dialog."));

    QDir dir;
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Choose a crossword puzzle to open"),
                                                dir.absolutePath().append(m_DefaultSaveFolder),
                                                tr("Crossword 3D (*.xwc3d);;Crossword Compiler (*.xwc)"));
    if(!path.isNull())
    {
        QFileInfo fileInfo(path);
        QString extension = fileInfo.suffix();

        m_PuzzleLoader.loadPuzzle(*m_Puzzle, path, extension);

        emit puzzleLoaded();
    }
}

void MainWindow::saveCrossword()
{
    ITextToSpeech::instance().speak(tr("Opening save crossword dialog."));

    QDir dir;
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("Save as"),
                                                dir.absolutePath().append(m_DefaultSaveFolder),
                                                tr("Crossword 3D (*.xwc3d);;Crossword Compiler (*.xwc)"));

    if(!path.isNull())
    {
        QFileInfo fileInfo(path);
        QString extension = fileInfo.suffix();

        m_PuzzleLoader.savePuzzle(*m_Puzzle, path, extension);
    }
}

void MainWindow::showFileProperties()
{
    ITextToSpeech::instance().speak(m_Puzzle->getInformationString());
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

void MainWindow::cycleTableViewFilter()
{
    static unsigned int s_Filter = 0;
    QRegExp filterUnstarted = QRegExp("[]", Qt::CaseInsensitive, QRegExp::FixedString);
    QRegExp filterCompleted = QRegExp("", Qt::CaseInsensitive, QRegExp::FixedString);
    QRegExp filterNone = QRegExp("", Qt::CaseInsensitive, QRegExp::FixedString);

    switch(s_Filter)
    {
        case 0:
            m_ProxyModel->setFilterRegExp(filterCompleted);
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak(tr("Filtering unstarted crossword entries."));
            break;
        case 1:
            m_ProxyModel->setFilterRegExp(filterUnstarted);
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak(tr("Filtering completed crossword entries."));
            break;
        case 2:
             m_ProxyModel->setFilterRegExp(filterNone);
             m_ProxyModel->setFilterKeyColumn(1);
             ITextToSpeech::instance().speak(tr("Filtering disabled."));
            break;
    }

    s_Filter++;
    if(s_Filter > 2)
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

void MainWindow::showError(QString title, QString error)
{
    ITextToSpeech::instance().speak(QString(title).append(" - ").append(error));
}

void MainWindow::createShortcuts()
{
    m_LoadShortcutKey = Qt::Key_L;
    m_SaveShortcutKey = Qt::Key_S;
    m_HelpShortcutKey = Qt::Key_H;
    m_ExitShortcutKey = Qt::Key_Q;
    m_ScoreShortcutKey = Qt::Key_K;
    m_FilePropertiesShortcutKey = Qt::Key_P;
    m_FilterTableViewShortcutKey = Qt::Key_F;

    m_LoadShortcut = new QShortcut(QKeySequence(m_LoadShortcutKey), this);
    connect(m_LoadShortcut, SIGNAL(activated()), this, SLOT(loadCrossword()));

    m_SaveShortcut = new QShortcut(QKeySequence(m_SaveShortcutKey), this);
    connect(m_SaveShortcut, SIGNAL(activated()), this, SLOT(saveCrossword()));

    m_HelpShortcut = new QShortcut(QKeySequence(m_HelpShortcutKey), this);
    connect(m_HelpShortcut, SIGNAL(activated()), this, SLOT(openHelp()));

    m_ExitShortcut = new QShortcut(QKeySequence(m_ExitShortcutKey), this);
    connect(m_ExitShortcut, SIGNAL(activated()), this, SLOT(exitConfirmation()));

    m_ScoreShortcut = new QShortcut(QKeySequence(m_ScoreShortcutKey), this);
    connect(m_ScoreShortcut, SIGNAL(activated()), this, SLOT(scoreCrossword()));

    m_FilePropertiesShortcut = new QShortcut(QKeySequence(m_FilePropertiesShortcutKey), this);
    connect(m_FilePropertiesShortcut, SIGNAL(activated()), this, SLOT(showFileProperties()));

    m_FilterTableViewShortcut = new QShortcut(QKeySequence(m_FilterTableViewShortcutKey), this);
    connect(m_FilterTableViewShortcut, SIGNAL(activated()), this, SLOT(cycleTableViewFilter()));
}

QString MainWindow::getIntroString()
{
    return QString(tr("Welcome to BCrossword 3D. Press ").append(m_LoadShortcutKey).append(" to load a crossword. "))
            .append("Press ").append(m_ExitShortcutKey).append(" to quit the program. ")
            .append("Press ").append(m_HelpShortcutKey).append(" to open a help document in your web browser. Use your screen reader to read the document");
}

void MainWindow::showAbout()
{
    ITextToSpeech::instance().speak(tr("BCrossword3D is a 2D and 3D crossword puzzle game for the visually impaired."));
}

void MainWindow::exitConfirmation()
{
    close();
}
