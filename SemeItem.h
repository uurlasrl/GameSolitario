//
// Created by Domenico Neri on 07/11/21.
//

#ifndef GAMESOLITARIO_SEMEITEM_H
#define GAMESOLITARIO_SEMEITEM_H

#include "card.h"
/**
 * gestisce le carte di un seme
 */
class SemeItem: public CardStackItem{

public:
    explicit SemeItem(float wi,float hi,int id, QColor *color,QGraphicsView *parentview);

    bool isValid(Card* card) override;

    QRectF boundingRect() const override;

    void setBoardSize(QSize);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool isCardDragableAt(QPointF point) override;

    CardList getDragingCard(QPointF point) override;
private:
    float myw;
    float myh;
    int m_id;
};


#endif //GAMESOLITARIO_SEMEITEM_H
