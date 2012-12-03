#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QSortFilterProxyModel>
#include <QTextStream>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextEdit>

#include "puzzlebase.h"

#include "grid3dgraphicsscene.h"
#include "wordtablemodel.h"
#include "itexttospeech.h"
#include "quitdialog.h"
#include "filedialog.h"
#include "shortcutkeys.h"

const QString MainWindow::m_DefaultSaveFolder = QString("/Crosswords");
const QString MainWindow::m_HelpFileLocation = QString("/Help/index.html");
const QString MainWindow::m_LicenseFileLocation = QString("/License/gplv3.htm");
const QString MainWindow::m_EmailAddressFileLocation = QString("/Config/email_address.txt");
const QString MainWindow::m_PostalAddressFileLocation = QString("/Config/postal_address.txt");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon.ico"));

    createShortcuts();

    m_GraphicsScene = new Grid3DGraphicsScene(m_Puzzle);
    ui->graphicsView->setScene(m_GraphicsScene);

    m_WordTableModel = new WordTableModel(m_Puzzle, m_Puzzle.getRefCrosswordEntries());
    m_ProxyModel = new QSortFilterProxyModel(this);
    m_ProxyModel->setSourceModel(m_WordTableModel);
    ui->wordTableView->setModel(m_ProxyModel);

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

        if(m_PuzzleLoader.loadPuzzle(m_Puzzle, path, extension))
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
        ITextToSpeech::instance().speak(tr("Crossword was saved as: ")
                                        .append(updatedFileInfo.fileName())
                                        .append(". In folder: ")
                                        .append(updatedFileInfo.filePath()));
    }
}

void MainWindow::printCrossword()
{
    ITextToSpeech::instance().speak("Attempting to open a print dialogue for printing your answers.");

    QString postalAddress = "Error fetching postal address, please check the game Config folder to find the postal address.";

    QDir dir;
    QString filePath;
    if(dir.exists(dir.absolutePath().append(m_PostalAddressFileLocation)))
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
        }

        QTextStream in(&file);
        QString currentLine;
        QStringList linelist;

        if(in.atEnd())
        {
        }

        do
        {
            currentLine = in.readLine();
            if(currentLine.length() != 0)
            {
                linelist << currentLine;
            }
        } while (!currentLine.isNull());

        while(!linelist.isEmpty())
        {
            postalAddress.append(linelist.takeFirst()).append("\n");
        }
    }

    QString textToPrint;

    textToPrint.append(m_Puzzle.getInformationString().append("\n\n"));

    for(unsigned int i = 0; i < m_Puzzle.getRefCrosswordEntries().size(); i++)
    {
        QString id = m_Puzzle.getRefCrosswordEntries().at(i).getEntryName();
        QString direction = m_Puzzle.getRefCrosswordEntries().at(i).getDirection().getString();
        QString answer = m_Puzzle.getRefCrosswordEntries().at(i).getGuess().getString();

        textToPrint.append(id).append(" ").append(direction).append(" --- ").append(answer).append("\n");
    }

    textToPrint.append("\n").append(postalAddress);

    QTextEdit textViewer(textToPrint);
    QTextDocument* document = textViewer.document();

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted)
    {
        document->print(&printer);
        ITextToSpeech::instance().speak("Sending print request to printer.");
    }
}

void MainWindow::emailCrossword()
{
    QString emailAddress = "enter@your.email_address_here.com";
    QString emailSubject = m_Puzzle.getPuzzleTitle().append(" answers");

    QDir dir;
    QString filePath;
    if(dir.exists(dir.absolutePath().append(m_EmailAddressFileLocation)))
    {
        filePath = dir.absolutePath().append(m_EmailAddressFileLocation);

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {

        }

        QTextStream in(&file);
        QString currentLine;
        QStringList linelist;

        if(in.atEnd())
        {
        }

        do
        {
            currentLine = in.readLine();
            if(currentLine.length() != 0)
            {
                linelist << currentLine;
            }
        } while (!currentLine.isNull());

        if(!linelist.isEmpty())
        {
            emailAddress = linelist.takeFirst();
        }
    }

    QString emailBody;

    emailBody.append(m_Puzzle.getInformationString().append("%0A%0A"));

    for(unsigned int i = 0; i < m_Puzzle.getRefCrosswordEntries().size(); i++)
    {
        QString id = m_Puzzle.getRefCrosswordEntries().at(i).getEntryName();
        QString direction = m_Puzzle.getRefCrosswordEntries().at(i).getDirection().getString();
        QString answer = m_Puzzle.getRefCrosswordEntries().at(i).getGuess().getString();

        emailBody.append(id).append(" ").append(direction).append(" --- ").append(answer).append("%0A");
    }

    QUrl mailtoURL = QUrl(QString("mailto:").append(emailAddress)
                          .append("?subject=").append(emailSubject)
                          .append("&body=").append(emailBody));

    if(QDesktopServices::openUrl(mailtoURL))
    {
        ITextToSpeech::instance().speak("Attempting to open an email containing your answers.");
    }
    else
    {
        ITextToSpeech::instance().speak("Failed to open an email containing your answers.");
    }
}

void MainWindow::emailFeedback()
{
    QString emailAddress = "enter@your.email_address_here.com";
    QString emailSubject = m_Puzzle.getPuzzleTitle().append("Blind Crossword 3D feedback");

    QDir dir;
    QString filePath;
    if(dir.exists(dir.absolutePath().append(m_EmailAddressFileLocation)))
    {
        filePath = dir.absolutePath().append(m_EmailAddressFileLocation);

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {

        }

        QTextStream in(&file);
        QString currentLine;
        QStringList linelist;

        if(in.atEnd())
        {
        }

        do
        {
            currentLine = in.readLine();
            if(currentLine.length() != 0)
            {
                linelist << currentLine;
            }
        } while (!currentLine.isNull());

        if(!linelist.isEmpty())
        {
            emailAddress = linelist.takeFirst();
        }
    }

    QString emailBody;

    emailBody.append(m_Puzzle.getInformationString().append("%0A%0A"));

    QUrl mailtoURL = QUrl(QString("mailto:").append(emailAddress)
                          .append("?subject=").append(emailSubject)
                          .append("&body=").append(emailBody));

    if(QDesktopServices::openUrl(mailtoURL))
    {
        ITextToSpeech::instance().speak("Attempting to open an email for you to send us feedback.");
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
    ITextToSpeech::instance().speak(m_Puzzle.getPuzzleThemePhrase());
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
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak("Showing unstarted crossword entries.");
            break;
        case 1:
            m_ProxyModel->setFilterRegExp(showCompleted);
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak("Showing completed crossword entries.");
            break;
        case 2:
            m_ProxyModel->setFilterRegExp(showPartial);
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak("Showing partially completed crossword entries.");
            break;
        case 3:
             m_ProxyModel->setFilterRegExp(showAll);
             m_ProxyModel->setFilterKeyColumn(1);
             ITextToSpeech::instance().speak("Filtering disabled.");
            break;
       case 4:
            m_ProxyModel->setFilterRegExp(showIncidentCrosswordEntries);
            m_ProxyModel->setFilterKeyColumn(1);
            ITextToSpeech::instance().speak("Showing all clues that touch the current clue");
            break;
    }

    s_Filter++;
    if(s_Filter >= cs_NumFilters)
    {
        s_Filter = 0;
    }
}

void MainWindow::checkIfPuzzleWasCompleted()
{
    if(m_Puzzle.getPuzzleType() != CrosswordTypes::WITHOUT_ANSWERS)
    {
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
    ITextToSpeech::instance().speak(m_Puzzle.getScoreString());
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
