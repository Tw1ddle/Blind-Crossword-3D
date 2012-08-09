#ifndef GRAPHICSGRIDITEM_H
#define GRAPHICSGRIDITEM_H

#include <QGraphicsItem>

#include "uivec3.h"
#include "letter.h"

class GraphicsGridItem : public QGraphicsItem
{
public:
    explicit GraphicsGridItem(Letter letter, unsigned int gridId, QGraphicsItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setCrosswordEntryNumber(unsigned int entryNumber);
    void setColor(QColor color);
    QRectF boundingRect() const;

    Letter getLetter();

    static const unsigned int s_Size = 50u;

private:
    QColor m_Color;
    Letter m_Letter;
    QString m_CrosswordEntryNumber;
    unsigned int m_GridId;

    static const float s_CrosswordLetterScale;
    static const float s_CrosswordEntryNumberScale;
};

#endif // GRAPHICSGRIDITEM_H
