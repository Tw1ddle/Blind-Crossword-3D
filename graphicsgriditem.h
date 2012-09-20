#ifndef GRAPHICSGRIDITEM_H
#define GRAPHICSGRIDITEM_H

#include <QGraphicsItem>

#include "uivec3.h"
#include "letter.h"

class GraphicsGridItem : public QGraphicsItem
{
public:
    GraphicsGridItem(Letter* letter, unsigned int gridId, QGraphicsItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setCrosswordEntryNumber(unsigned int entryNumber);
    void setColor(QColor color);

    QString getCrosswordEntryNumber() const;

    QRectF boundingRect() const;

    static const unsigned int sc_Size = 50u;
private:
    QColor m_Color;
    Letter* m_LetterRef;
    QString m_CrosswordEntryNumber;
    unsigned int m_GridId;

    static const float sc_CrosswordLetterScale;
    static const float sc_CrosswordEntryNumberScale;
};

#endif // GRAPHICSGRIDITEM_H
