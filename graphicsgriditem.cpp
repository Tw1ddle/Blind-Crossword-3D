#include "graphicsgriditem.h"

#include "letter.h"
#include <QFont>
#include <QPainter>

const float GraphicsGridItem::s_CrosswordLetterScale = 0.8f;
const float GraphicsGridItem::s_CrosswordEntryNumberScale = 0.20f;

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
    return QRectF(0, 0, s_Size, s_Size);
}

Letter GraphicsGridItem::getLetter()
{
    return m_Letter;
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

    painter->drawRect(0, 0, s_Size, s_Size);

    pen.setColor(Qt::black);
    painter->setPen(pen);

    if(m_Letter.getChar() != QChar())
    {
        QFont font;
        font.setFamily("Georgia");
        font.setPixelSize(s_Size * s_CrosswordLetterScale);
        painter->setFont(font);
        painter->drawText(boundingRect(), Qt::AlignCenter, m_Letter.getChar());
    }

    if(!m_CrosswordEntryNumber.isNull())
    {
        QFont font;
        font.setFamily("Georgia");
        font.setPixelSize(s_Size * s_CrosswordEntryNumberScale);
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
