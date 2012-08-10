#include "graphicsgriditem.h"

#include <QFont>
#include <QPainter>

#include "letter.h"

const float GraphicsGridItem::sc_CrosswordLetterScale = 0.8f;
const float GraphicsGridItem::sc_CrosswordEntryNumberScale = 0.30f;

GraphicsGridItem::GraphicsGridItem(Letter letter, unsigned int gridId, QGraphicsItem *parent) :
    QGraphicsItem(parent), m_Letter(letter)
{
    m_GridId = gridId;

    if(letter.getChar() == QChar())
    {
        m_Color = QColor(Qt::gray);
    }
    else
    {
        m_Color = QColor(Qt::lightGray);
    }
}

QRectF GraphicsGridItem::boundingRect() const
{
    return QRectF(0, 0, sc_Size, sc_Size);
}

void GraphicsGridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(m_Color);
    painter->setBrush(QBrush(m_Color));
    painter->setPen(pen);

    painter->drawRect(0, 0, sc_Size, sc_Size);

    pen.setColor(Qt::black);
    painter->setPen(pen);

    if(!m_Letter.getChar().isNull())
    {
        QFont font;
        font.setFamily("Georgia");
        font.setPixelSize(sc_Size * sc_CrosswordLetterScale);
        painter->setFont(font);
        painter->drawText(boundingRect(), Qt::AlignCenter, m_Letter.getChar());
    }

    if(!m_CrosswordEntryNumber.isNull())
    {
        QFont font;
        font.setFamily("Georgia");
        font.setPixelSize(sc_Size * sc_CrosswordEntryNumberScale);
        painter->setFont(font);
        painter->drawText(boundingRect(), Qt::AlignTop | Qt::AlignLeft, m_CrosswordEntryNumber);
    }
}

void GraphicsGridItem::setCrosswordEntryNumber(unsigned int entryNumber)
{
    m_CrosswordEntryNumber = QString::number(entryNumber);
}

void GraphicsGridItem::setColor(QColor color)
{
    m_Color = color;
}
