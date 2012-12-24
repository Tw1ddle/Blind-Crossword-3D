#include "filedialog.h"

#include <QFileInfo>
#include <QListView>
#include <QBoxLayout>

#include "itexttospeech.h"
FileDialog::FileDialog(QWidget* parent, const QString& caption, const QString& directory, const QString& nameFilter, const QStringList& filter)
{
    Q_UNUSED(nameFilter);
    Q_UNUSED(parent);

    m_Model = new QFileSystemModel(this);
    m_View = new FileListView(this);

    setWindowTitle(caption);
    setWindowFlags(Qt::Dialog);

    m_Model->setFilter(QDir::Files);
    m_Model->setNameFilters(filter);
    m_Model->setRootPath(directory);

    m_View->setIconSize(QSize(0, 0)); //todo check this hides the icons everywhere

    m_Model->setNameFilterDisables(false);

    m_View->setModel(m_Model);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_View);
    setLayout(layout);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);

    m_View->setRootIndex(m_Model->index(directory));
    m_View->setFocus();
    m_View->setMinimumWidth(640);
    m_View->setMinimumHeight(480);

    connect(m_View, SIGNAL(selectedItemChanged(QModelIndex, QModelIndex)), this, SLOT(updateCurrent(QModelIndex, QModelIndex)));
    connect(m_View, SIGNAL(activated(QModelIndex)), this, SLOT(onFileSelected(QModelIndex)));
    connect(m_View, SIGNAL(clicked(QModelIndex)), this, SLOT(onSelectionChanged(QModelIndex)));
}

FileDialog::~FileDialog()
{
    delete m_Model;
    delete m_View;
}

QString FileDialog::getSelectedFilePath() const
{
    return m_CurrentFilePath;
}

void FileDialog::onSelectionChanged(const QModelIndex& current)
{
    m_CurrentFilePath = m_Model->filePath(current);

    QString fileName = m_Model->fileInfo(current).baseName(); // read the file name without the extension
    ITextToSpeech::instance().speak(fileName);
}

FileListView::FileListView(QWidget *parent) : QListView(parent)
{
    setFont(QFont("Lucida Console", 20, -1, false));
}

void FileListView::keyPressEvent(QKeyEvent *event)
{
    QModelIndex oldIdx = currentIndex();
    QListView::keyPressEvent(event);
    QModelIndex newIdx = currentIndex();
    if(oldIdx.row() != newIdx.row())
    {
        emit clicked(newIdx);
    }
}

void FileDialog::onFileSelected(const QModelIndex &index)
{
    m_CurrentFilePath = m_Model->filePath(index);
    accept();
}

