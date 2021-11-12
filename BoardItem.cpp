//
// Created by Domenico Neri on 07/11/21.
//

#include "BoardItem.h"


BoardItem::BoardItem(CardList cards) {
    for(int i=0;i<cards.size();i++){
        if(i==0)
            carteScoperte.push(cards[0]);
        else
            carteCoperte.push(cards[i]);
    }

}
bool BoardItem::isValid(Card *card) {
    return carteScoperte.isEmpty() && carteScoperte.last()->getCardNumber()+1==card->getCardNumber();
}