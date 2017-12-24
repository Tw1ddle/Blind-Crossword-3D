/*! \brief Models a grid item within the graphical grid view.
 *
 *  References a letter in the crossword grid.
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

#include <QGraphicsItem>

#include "uivec3.h"
#include "letter.h"

class GraphicalGridItem : public QGraphicsItem
{
public:
    GraphicalGridItem(const Letter* letter, unsigned int gridId, QGraphicsItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setCrosswordEntryNumber(unsigned int entryNumber);
    void setColor(QColor color);
    QColor getColor() const;
    void setLetter(const Letter* letter);

    QString getCrosswordEntryNumber() const;

    QRectF boundingRect() const;

    static const unsigned int sc_Size = 50u;
private:
    const Letter* m_LetterRef;
    QColor m_Color;
    QString m_CrosswordEntryNumber;
    unsigned int m_GridId;

    static const float sc_CrosswordLetterScale;
    static const float sc_CrosswordEntryNumberScale;
};
