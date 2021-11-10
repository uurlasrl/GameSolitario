//
// Created by Domenico Neri on 07/11/21.
//

#include "BoardItem.h"

bool BoardItem::isValid(Card *card) {
    return carteScoperte.isEmpty() && carteScoperte.last()->getCardNumber()+1==card->getCardNumber();
}