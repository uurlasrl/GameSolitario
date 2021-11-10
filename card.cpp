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

CardStackItem::CardStackItem(){
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
    QList<Card*> temp;
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
QList<Card*> CardStackItem::getCarteScoperte() const{
    QList<Card*> tmp(carteScoperte);
    return tmp;
}

