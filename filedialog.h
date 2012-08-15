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
    
    QString getSelectedFilePath();

private:
    QFileSystemModel* m_Model;
    FileListView* m_View;
    QString m_CurrentFilePath;
    
public slots:
    void onSelectionChanged(const QModelIndex& current);
    void onFileSelected(const QModelIndex& index);
};

#endif // LOADCROSSWORDDIALOG_H
