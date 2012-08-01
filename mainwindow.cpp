#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "puzzle3d.h"
#include "grid3dgraphicsscene.h"

#include <QFileDialog>

const QString MainWindow::m_DefaultSaveFolder = QString("/Crosswords");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_Puzzle = new Puzzle3D();
    m_GraphicsScene = new Grid3DGraphicsScene(&m_Puzzle->getDimensions(), &m_Puzzle->getWorkingLetters());

    ui->graphicsView->setScene(m_GraphicsScene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFile()
{
    QDir dir;
    QString res = QFileDialog::getOpenFileName(this, tr("Choose a crossword puzzle to open"), dir.absolutePath().append(m_DefaultSaveFolder), tr("Crossword Puzzle File (*.xwc3d *.xwc)"));
    if(!res.isNull())
    {
        m_PuzzleLoader.loadPuzzle(*m_Puzzle, res);
    }
}

void MainWindow::saveFile()
{
    QDir dir;
    QString res = QFileDialog::getSaveFileName(this, tr("Choose file to save"), dir.absolutePath().append(m_DefaultSaveFolder), tr("Crossword Puzzle File (*.xwc3d)"));
    if(!res.isNull())
    {
        m_PuzzleLoader.savePuzzle(*m_Puzzle, res);
    }
}
