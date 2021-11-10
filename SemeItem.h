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
    bool isValid(Card* card) override;
    bool transferFrom(CardStackItem *otherCardStack, Card *from) override;
};


#endif //GAMESOLITARIO_SEMEITEM_H
