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
Q_OBJECT
public:
    virtual ~SemeItem();
    explicit SemeItem(float wi,float hi,int id, QColor *color,QGraphicsView *parentview);

    //controllo del gioco
    bool isValid(Card* card) override;

    //ridimensionamento delle carte sul evento di resize della finestra
    void setBoardSize(QSize);

    //gestione evento drag&drop
    bool isCardDragableAt(QPointF point) override;
    CardList getDragingCard(QPointF point) override;

// richieste dall'Item per la gestione della visualizzazione
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    float myw;
    float myh;
    int m_id;
};


#endif //GAMESOLITARIO_SEMEITEM_H
