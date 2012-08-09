#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "puzzle3d.h"
#include "grid3dgraphicsscene.h"
#include "wordtablemodel.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include "itexttospeech.h"

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
    ui->wordTableView->setModel(m_WordTableModel);


    connect(this, SIGNAL(puzzleLoaded()), m_WordTableModel, SLOT(crosswordEntriesChanged()));
    connect(this, SIGNAL(puzzleLoaded()), m_GraphicsScene, SLOT(buildPuzzleGrid()));

    connect(ui->wordTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(tableSelectionChanged(QItemSelection)));
    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString, QModelIndex)), m_WordTableModel, SLOT(guessEntered(QString, QModelIndex)));

    connect(m_WordTableModel, SIGNAL(guessValidated()), m_GraphicsScene, SLOT(buildPuzzleGrid()));
    connect(m_WordTableModel, SIGNAL(conflictingWordError()), ui->wordTableView, SLOT(conflictingWordError()));

    connect(&m_PuzzleLoader, SIGNAL(loaderError(QString, QString)), this, SLOT(showError(QString, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();

    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("Quit BCrossword 3D");
    msgBox->setText("Do you really want to quit?");

    QPushButton *yesButton = msgBox->addButton(tr("Yes"), QMessageBox::ActionRole);
    msgBox->addButton(tr("No"), QMessageBox::ActionRole);
    msgBox->exec();

    if ((QPushButton*)msgBox->clickedButton() == yesButton)
    {
        event->accept();
    }
}

void MainWindow::loadFile()
{
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

void MainWindow::saveFile()
{
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
    m_Puzzle->showInformation(this);
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

    if(!openedSuccessfully)
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not open license document"));
    }
}

void MainWindow::openHelp()
{
    ITextToSpeech::instance().speak(QString("Able bodied cows eat the geass"));
    QDir dir;
    QString filePath = dir.absolutePath().append(m_HelpFileLocation);
    QUrl url = QUrl::fromLocalFile(filePath);

    QDesktopServices::openUrl(url);
}

void MainWindow::showAboutScreen()
{
    QMessageBox::information(this, tr("About BCrossword3D"), tr("BCrossword3D is a 2D and 3D crossword puzzle game for the visually impaired."));
}

void MainWindow::scoreCrossword()
{
    unsigned int score = m_Puzzle->scoreSolution();



    QMessageBox::information(this, tr("Score"), tr("Current score: ").append(QString::number(score)).append("."));
}

void MainWindow::showError(QString title, QString error)
{
    QMessageBox::critical(this, title, error);
}

void MainWindow::exitConfirmation()
{
    close();
}

void MainWindow::createShortcuts()
{
    m_LoadShortcut = new QShortcut(QKeySequence(tr("L")), this);
    connect(m_LoadShortcut, SIGNAL(activated()), this, SLOT(loadFile()));

    m_SaveShortcut = new QShortcut(QKeySequence(tr("S")), this);
    connect(m_SaveShortcut, SIGNAL(activated()), this, SLOT(saveFile()));

    m_HelpShortcut = new QShortcut(QKeySequence(tr("H")), this);
    connect(m_HelpShortcut, SIGNAL(activated()), this, SLOT(openHelp()));

    m_ExitShortcut = new QShortcut(QKeySequence(tr("Q")), this);
    connect(m_ExitShortcut, SIGNAL(activated()), this, SLOT(exitConfirmation()));

    m_ScoreShortcut = new QShortcut(QKeySequence(tr("K")), this);
    connect(m_ScoreShortcut, SIGNAL(activated()), this, SLOT(scoreCrossword()));

    m_FilePropertiesShortcut = new QShortcut(QKeySequence(tr("P")), this);
    connect(m_FilePropertiesShortcut, SIGNAL(activated()), this, SLOT(showFileProperties()));
}
