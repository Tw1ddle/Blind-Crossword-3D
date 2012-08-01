#ifndef WORDLISTVIEW_H
#define WORDLISTVIEW_H

#include <QListWidget>
#include "crosswordentry3d.h"

class WordListView : public QListWidget
{
    Q_OBJECT
public:
    WordListView();

private:
    std::vector<CrosswordEntry3D>* m_RefCrosswordEntries;
    
signals:
    
public slots:
    
};

#endif // WORDLISTVIEW_H
