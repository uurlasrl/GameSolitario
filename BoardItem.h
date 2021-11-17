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

    //controllo del gioco
    bool isValid(Card *card) override;
    void scopriCartaIfEmpty(qint32 eventID=0) override;

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
    int itemCapacity;
};



#endif //GAMESOLITARIO_BOARDITEM_H
