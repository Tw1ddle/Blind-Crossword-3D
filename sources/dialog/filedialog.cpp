#include "dialog/filedialog.h"

#include <QBoxLayout>
#include <QFileInfo>
#include <QListView>

#include "tts/itexttospeech.h"

FileDialog::FileDialog(QWidget* parent, const QString& caption, const QString& directory,
                       const QString& nameFilter, const QStringList& filter)
{
    Q_UNUSED(nameFilter);
    Q_UNUSED(parent);

    m_model = new QFileSystemModel(this);
    m_view = new FileListView(this);

    setWindowTitle(caption);
    setWindowFlags(Qt::Dialog);

    m_model->setFilter(QDir::Files);
    m_model->setNameFilters(filter);
    m_model->setRootPath(directory);

    m_view->setIconSize(QSize(0, 0)); //todo check this hides the icons everywhere

    m_model->setNameFilterDisables(false);

    m_view->setModel(m_model);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_view);
    setLayout(layout);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);

    m_view->setRootIndex(m_model->index(directory));
    m_view->setFocus();
    m_view->setMinimumWidth(640);
    m_view->setMinimumHeight(480);

    connect(m_view, SIGNAL(selectedItemChanged(QModelIndex, QModelIndex)), this,
            SLOT(updateCurrent(QModelIndex, QModelIndex)));
    connect(m_view, SIGNAL(activated(QModelIndex)), this, SLOT(onFileSelected(QModelIndex)));
    connect(m_view, SIGNAL(clicked(QModelIndex)), this, SLOT(onSelectionChanged(QModelIndex)));
}

FileDialog::~FileDialog()
{
    delete m_model;
    delete m_view;
}

QString FileDialog::getSelectedFilePath() const
{
    return m_currentFilePath;
}

void FileDialog::onSelectionChanged(const QModelIndex& current)
{
    m_currentFilePath = m_model->filePath(current);

    QString fileName = m_model->fileInfo(
                           current).completeBaseName(); // read the file name without the extension
    ITextToSpeech::instance().speak(fileName);
}

FileListView::FileListView(QWidget* parent) : QListView(parent)
{
    setFont(QFont("Lucida Console", 20, -1, false));
}

void FileListView::keyPressEvent(QKeyEvent* event)
{
    QModelIndex oldIdx = currentIndex();
    QListView::keyPressEvent(event);
    QModelIndex newIdx = currentIndex();

    if (oldIdx.row() != newIdx.row()) {
        emit clicked(newIdx);
    }
}

void FileDialog::onFileSelected(const QModelIndex& index)
{
    m_currentFilePath = m_model->filePath(index);
    accept();
}

