#include "dialog/graphicalgriditem.h"

#include <QFont>
#include <QPainter>

#include "crossword/letter.h"

const float GraphicalGridItem::CROSSWORD_LETTER_SCALE = 0.8f;
const float GraphicalGridItem::CROSSWORD_ENTRY_NUMBER_SCALE = 0.30f;

GraphicalGridItem::GraphicalGridItem(const Letter* letter, unsigned int gridId,
                                     QGraphicsItem* parent) :
    QGraphicsItem(parent), m_letterRef(letter)
{
    m_gridId = gridId;

    if (letter->getChar() != QChar()) {
        setColor(Qt::white);
    } else {
        setColor(QColor(20, 20, 20));
    }
}

QRectF GraphicalGridItem::boundingRect() const
{
    return QRectF(0, 0, SIZE, SIZE);
}

void GraphicalGridItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                              QWidget* widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(m_color);
    painter->setBrush(QBrush(m_color));
    painter->setPen(pen);

    painter->drawRect(0, 0, SIZE, SIZE);

    pen.setColor(Qt::black);
    painter->setPen(pen);

    if (!m_letterRef->getChar().isNull()) {
        QFont font;
        font.setFamily("Georgia");
        font.setPixelSize(SIZE * CROSSWORD_LETTER_SCALE);
        painter->setFont(font);
        painter->drawText(boundingRect(), Qt::AlignCenter, m_letterRef->getChar());
    }

    if (!m_crosswordEntryNumber.isNull()) {
        QFont font;
        font.setFamily("Georgia");
        font.setPixelSize(SIZE * CROSSWORD_ENTRY_NUMBER_SCALE);
        painter->setFont(font);
        painter->drawText(boundingRect(), Qt::AlignTop | Qt::AlignLeft, m_crosswordEntryNumber);
    }
}

void GraphicalGridItem::setCrosswordEntryNumber(unsigned int entryNumber)
{
    m_crosswordEntryNumber = QString::number(entryNumber);
}

void GraphicalGridItem::setColor(QColor color)
{
    m_color = color;
}

QColor GraphicalGridItem::getColor() const
{
    return m_color;
}

void GraphicalGridItem::setLetter(const Letter* letter)
{
    m_letterRef = letter;
}

QString GraphicalGridItem::getCrosswordEntryNumber() const
{
    return m_crosswordEntryNumber;
}
