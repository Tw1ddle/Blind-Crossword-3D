/*! \brief Classes for viewing a list of files in the context of a filesystem.
 *  Provide a selectable list of files. Used for choosing a crossword to load.
 */

#pragma once

#include <QDialog>
#include <QFileSystemModel>
#include <QListView>

class FileListView : public QListView {
  public:
    FileListView(QWidget* parent);

    void keyPressEvent(QKeyEvent* event);
};

class FileDialog : public QDialog {
    Q_OBJECT
  public:
    FileDialog(QWidget* parent = 0, const QString& caption = QString(),
               const QString& directory = QString(), const QString& nameFilter = QString(),
               const QStringList& filter = QStringList());
    ~FileDialog();

    QString getSelectedFilePath() const;

  public slots:
    void onSelectionChanged(const QModelIndex& current);
    void onFileSelected(const QModelIndex& index);

  private:
    QFileSystemModel* m_model;
    FileListView* m_view;
    QString m_currentFilePath;
};
