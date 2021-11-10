//
// Created by Domenico Neri on 07/11/21.
//

#include "SemeItem.h"

bool SemeItem::isValid(Card *card) {
    if(carteScoperte.isEmpty())
        return card->getCardNumber() == 1;
    return carteScoperte.last()->getCardNumber()+1 == card->getCardNumber() && carteScoperte[0]->getCardColor() == card->getCardColor();
}

bool SemeItem::transferFrom(CardStackItem *otherCardStack, Card *from) {
    if(otherCardStack->sizeScoperte()>0&&otherCardStack->getCarteScoperte().last()==from)
        return CardStackItem::transferFrom(otherCardStack, from);
    return true;
}