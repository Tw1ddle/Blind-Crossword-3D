#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "puzzle3d.h"
#include "grid3dgraphicsscene.h"
#include "wordtablemodel.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>

const QString MainWindow::m_DefaultSaveFolder = QString("/Crosswords");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createShortcuts();

    m_Puzzle = new Puzzle3D();

    m_GraphicsScene = new Grid3DGraphicsScene(&m_Puzzle->getRefWorkingGrid(), &m_Puzzle->getRefCrosswordEntries());
    ui->graphicsView->setScene(m_GraphicsScene);

    m_WordTableModel = new WordTableModel(&m_Puzzle->getRefCrosswordEntries(), &m_Puzzle->getRefWorkingGrid());
    ui->wordTableView->setModel(m_WordTableModel);

    connect(this, SIGNAL(puzzleLoaded()), m_WordTableModel, SLOT(crosswordEntriesChanged()));
    connect(this, SIGNAL(puzzleLoaded()), m_GraphicsScene, SLOT(buildGrid()));

    connect(ui->wordTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(tableSelectionChanged(QItemSelection)));
    connect(ui->wordTableView, SIGNAL(guessSubmitted(QString, QModelIndex)), m_WordTableModel, SLOT(guessEntered(QString, QModelIndex)));
    connect(m_WordTableModel, SIGNAL(conflictingWordError()), ui->wordTableView, SLOT(intersectingWordError()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    //Setting parent will keep messagebox in the center of QMainWindow
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("Quit BCrossword 3D");
    msgBox->setText("Do you really want to quit?");

    QPushButton *yesButton = msgBox->addButton(tr("Yes"), QMessageBox::ActionRole);
    msgBox->addButton(tr("No"), QMessageBox::ActionRole);
    msgBox->exec();

    //If user clicks 'Yes'  button , accept QCloseEvent (Close Window)
    if ((QPushButton*)msgBox->clickedButton() == yesButton)
    {
    event->accept();
    }
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
}

void MainWindow::loadFile()
{
    QDir dir;
    QString path = QFileDialog::getOpenFileName(this, tr("Choose a crossword puzzle to open"), dir.absolutePath().append(m_DefaultSaveFolder), tr("Crossword Puzzle File (*.xwc3d *.xwc)"));
    if(!path.isNull())
    {
        m_PuzzleLoader.loadPuzzle(*m_Puzzle, path);

        emit puzzleLoaded();
    }
}

void MainWindow::saveFile()
{
    QDir dir;
    QString path = QFileDialog::getSaveFileName(this, tr("Choose file to save"), dir.absolutePath().append(m_DefaultSaveFolder), tr("Crossword Puzzle File (*.xwc3d)"));
    if(!path.isNull())
    {
        m_PuzzleLoader.savePuzzle(*m_Puzzle, path);
    }
}

void MainWindow::openHelp()
{
}

void MainWindow::exitConfirmation()
{
    close();
}

void MainWindow::tableSelectionChanged(const QItemSelection &)
{
}
