#include "wordtableview.h"

#include <QHeaderView>

WordTableView::WordTableView(QWidget *parent) :
    QTableView(parent)
{
    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}
