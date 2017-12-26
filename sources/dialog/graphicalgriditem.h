/*! \brief Models a grid item within the graphical grid view.
 *  References a letter in the crossword grid.
 */

#pragma once

#include <QColor>
#include <QGraphicsItem>

#include "crossword/letter.h"
#include "util/uivec3.h"

class GraphicalGridItem : public QGraphicsItem {
  public:
    GraphicalGridItem(const Letter* letter, unsigned int gridId, QGraphicsItem* parent = 0);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void setCrosswordEntryNumber(unsigned int entryNumber);
    void setColor(QColor color);
    QColor getColor() const;
    void setLetter(const Letter* letter);

    QString getCrosswordEntryNumber() const;

    QRectF boundingRect() const;

    static const unsigned int SIZE = 50u;

  private:
    const Letter* m_letterRef;
    QColor m_color;
    QString m_crosswordEntryNumber;
    unsigned int m_gridId;

    static const float CROSSWORD_LETTER_SCALE;
    static const float CROSSWORD_ENTRY_NUMBER_SCALE;
};
