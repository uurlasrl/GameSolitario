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

    public:
    BoardItem(CardList cards);

    bool isValid(Card *card) override;

    bool transferFrom(CardStackItem *otherCardStack, Card *from) override;

    QRectF boundingRect() const override;

    void setSize(QSize);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool isCardDragableAt(QPointF point) override;

    CardList getDragingCard(QPointF point) override;


private:

    float myw;

};



#endif //GAMESOLITARIO_BOARDITEM_H
