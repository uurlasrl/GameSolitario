//
// Created by Domenico Neri on 07/11/21.
//

#ifndef GAMESOLITARIO_CircolarCardItem_H
#define GAMESOLITARIO_CircolarCardItem_H

#include "card.h"
/**
 * @brief Mazzo di carte
 * proprio come un mazzo di carte produce oggetti cards
 *
 */
class CircolarCardItem: public CardStackItem{
public:
    CircolarCardItem(float ,QGraphicsView * , QColor *);

    Card *cardAvailable();

    //prende una carta dalle carte scoperte
    Card *getCard();

    // Card *getCard();
    Card *scopriCard();
    //riporta le carte nella posizione di partenza per ricominciare a scoprirle
    //se le carte dda scoprire non sono finite ritorna vero ma non modifica nulla
    // vero se l'operazione ha avuto successo, falso se le carte sono finite

    bool giraCards();
    // ripristina il gioco alla fase iniziale
    void resettaMazzo();
    // mischia le carte va fatta dopo aver resettato il mazzo
    void mischia();

    bool isValid(Card *card) override;

    bool transferFrom(CardStackItem *otherCardStack, Card *from) override;

    QRectF boundingRect() const override;

    void setSize(QSize);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QList<Card*> mazzo;
    int cardNumber;

    float myw;

};


#endif //GAMESOLITARIO_CircolarCardItem_H
