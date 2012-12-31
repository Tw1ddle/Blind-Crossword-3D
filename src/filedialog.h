/*! \brief Classes for viewing a list of files in the context of a filesystem.
 *
 *  Provide a selectable list of files. Used for choosing a crossword to load.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#ifndef LOADCROSSWORDDIALOG_H
#define LOADCROSSWORDDIALOG_H

#include <QListView>
#include <QFileSystemModel>
#include <QDialog>

class FileListView : public QListView
{
public:
    FileListView(QWidget* parent);

    void keyPressEvent(QKeyEvent *event);
};

class FileDialog : public QDialog
{
    Q_OBJECT
public:
    FileDialog(QWidget* parent = 0, const QString& caption = QString(), const QString& directory = QString(), const QString& nameFilter = QString(), const QStringList& filter = QStringList());
    ~FileDialog();

    QString getSelectedFilePath() const;

private:
    QFileSystemModel* m_Model;
    FileListView* m_View;
    QString m_CurrentFilePath;
    
public slots:
    void onSelectionChanged(const QModelIndex& current);
    void onFileSelected(const QModelIndex& index);
};

#endif // LOADCROSSWORDDIALOG_H
