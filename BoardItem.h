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

public:
    bool isValid(Card* card) override;
};



#endif //GAMESOLITARIO_BOARDITEM_H
