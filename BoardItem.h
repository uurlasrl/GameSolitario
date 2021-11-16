//
// Created by Domenico Neri on 07/11/21.
//

#ifndef GAMESOLITARIO_BOARDITEM_H
#define GAMESOLITARIO_BOARDITEM_H

#include "card.h"

/**
 * gestisce le carte da scoprire
 */
class BoardItem: public CardStackItem{
    Q_OBJECT
    public:
    virtual ~BoardItem();
    explicit BoardItem(float,QColor *color, CardList cards,QGraphicsView *parentview);

    bool isValid(Card *card) override;

    QRectF boundingRect() const override;

    void setBoardSize(QSize);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool isCardDragableAt(QPointF point) override;

    CardList getDragingCard(QPointF point) override;


private:

    float myw;
    float myh;
    int itemCapacity;
};



#endif //GAMESOLITARIO_BOARDITEM_H
