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

    CardList distributeCards(int number);

    Card *cardAvailable();

    //prende una carta dalle carte scoperte
    Card *getCard();
    Card *scopriCard();

    //riporta le carte nella posizione di partenza per ricominciare a scoprirle
    //se le carte dda scoprire non sono finite ritorna vero ma non modifica nulla
    // vero se l'operazione ha avuto successo, falso se le carte sono finite

    bool giraCards();
    // ripristina il gioco alla fase iniziale
    void resettaMazzo();
    // mischia le carte va fatta dopo aver resettato il mazzo
    void mischia();

    //controllo del gioco
    bool isValid(Card *card) override;

    //ridimensionamento delle carte sul evento di resize della finestra
    void setBoardSize(QSize);

    //gestione evento drag&drop
    bool isCardDragableAt(QPointF point) override;
    CardList getDragingCard(QPointF point) override;

    // richieste dall'Item per la gestione della visualizzazione
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



private:
    CardList mazzo;
    int cardNumber;

    float myw;
    float myh;

};


#endif //GAMESOLITARIO_CircolarCardItem_H
