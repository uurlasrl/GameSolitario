#include "card.h"
#include <algorithm>

/**
 * @brief Card::Card
 * inizializza la carta
 * @param iid
 */
Card::CardColor Card::getCardColor() {
    return cardColor;
}

int Card::getCardNumber() {
    return value + 1;
}

Card::Card(int iid) {
    id = iid;
    value = iid % 13;
    cardColor = static_cast<CardColor>(iid / 13);

}

void Card::paint(QRectF *position, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen=painter->pen();
    if(this->cardColor==CardColor::Cuori || this->cardColor==CardColor::Quadri){
        pen.setColor(Qt::red);
    }else{
        pen.setColor(Qt::black);
    }
    painter->setPen(pen);
    painter->drawRoundedRect(position->x()+3,position->y()+5,position->width()-6,position->height()-10,10.0,10.0);
    QPointF centro(position->center());
    painter->drawText(centro,QString::number(this->value));
    centro.setY(centro.y()+25);
    switch (this->cardColor) {
        case CardColor::Cuori:
            painter->drawText(centro,QString("cuori"));
            break;
        case CardColor::Quadri:
            painter->drawText(centro,QString("quadri"));
            break;
        case CardColor::Fiori:
            painter->drawText(centro,QString("fiori"));
            break;
        case CardColor::Picche:
            painter->drawText(centro,QString("picche"));
            break;

    }
}

CardStackItem::CardStackItem(QColor *c):color(c){
    rand_generator.seed(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

/**
 * controlla se lo stack e' vuoto
 * @return
 */
bool CardStackItem::isEmpty(){
    return carteScoperte.isEmpty()&&carteCoperte.isEmpty();
}

int CardStackItem::sizeCoperte(){
    return carteCoperte.size();
}
int CardStackItem::sizeScoperte(){
    return carteCoperte.size();
}

/**
 * @brief CardStackItem::transferFrom
 * trasferisce le carta da un altro stack
 * @param otherCardStack
 * @return torna vero se il trasferimanto e' andato a buon fine altrimenti falso
 */
bool CardStackItem::transferFrom(CardStackItem *otherCardStack, Card*from) {

    //va a cercare l'indice della card
    Card *c;
    int idx= otherCardStack->carteScoperte.size()-1;
    while(idx>=0){
        c=otherCardStack->carteScoperte[idx];
        if(c==from){
            break;
        }
        idx--;
    }
    if(idx<0)return false; //non trova la card
    if(!this->isValid(c)) return false; //l'elemento da inserire non e' valido

    // calcola il numero di card da togliere
    int len = idx = otherCardStack->carteScoperte.size() - idx;

    // riversa le card da togliere in una lista temporanea
    CardList temp;
    while(idx>0){
        temp.append(otherCardStack->carteScoperte.pop());
        idx--;
    }
    idx = len-1;
    while(idx>0) {
        this->carteScoperte.push(temp[--idx]);
    }

    // comunica le modifiche per un visitors o per la modifica della visualizzazione
    qint32 id = rand_generator.generate();
    emit changeData(id, 0,temp);//aggiunta carte
    emit otherCardStack->changeData(id, 1,temp);//rimozione carte

    return true;
}
CardList CardStackItem::getCarteScoperte() const{
    CardList tmp(carteScoperte);
    return tmp;
}

