/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
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
