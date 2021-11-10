//
// Created by Domenico Neri on 07/11/21.
//

#include "CircolarCardItem.h"

/**
 * @brief CircolarCardItem::CircolarCardItem
 * inizializza il mazzo di carte
 */

CircolarCardItem::CircolarCardItem(): CardStackItem() {
    mazzo.reserve(sizeof(Card *) * 52);
    for (int i = 0; i < 52; i++) {
        mazzo[i] = new Card(i);
    }
    mischia();
    resettaMazzo();
}

Card *CircolarCardItem::cardAvailable() {
    if (!this->carteScoperte.isEmpty()) {
        return this->carteCoperte.last();
    }
    return nullptr;
}

/**
 * @brief CircolarCardItem::getCard
 * prende una carta dal mazzo
 * se ci sono delle carte scoperte le prende da li altrimenti ritorna null
 * @return
 */
Card *CircolarCardItem::getCard() {
    if (carteScoperte.isEmpty()) {
        return nullptr;
    }
    return carteScoperte.pop();
}

/**
 * @brief CircolarCardItem::scopri
 * scopre una carte prendendola dalle carte coperte e inserendala sulle carte scoperte
 */
Card *CircolarCardItem::scopriCard() {
    Card *card;
    if (carteCoperte.isEmpty()) {
        return nullptr;
    }
    card = carteCoperte.pop();
    carteScoperte.push(card);
    qint32 id= rand_generator.generate();
    emit changeData(id,3,QList<Card*>({card})); //scopre carta

    return card;
}

/**
 * @brief CircolarCardItem::giraCards
 * riporta le carte nella posizione di partenza per ricominciare a scoprirle
 * @return
 */
bool CircolarCardItem::giraCards() {
    if (carteCoperte.isEmpty()) {
        if (!carteScoperte.isEmpty()) {
            //svuoto le carte scoperte e riempio di nuovo le carte coperte
            while (!carteScoperte.isEmpty())carteCoperte.push(carteScoperte.pop());
            qint32 id=rand_generator.generate();
            QList<Card*> temp(carteCoperte);
            carteScoperte.push(carteCoperte.pop());
            emit changeData(id,4,temp); //carte girate
        } else {
            // le carte coperte sono finite e anche le carte scoperte
            return false;
        }
    } else {
        //le carte coperte non sono finite si sta tentando di girare il mazzo
        qWarning() << "Attenzione le carte coperte non sono finite ma si sta tentando di girare il mazzo";
    }
    return true;
}

/**
 * @brief CircolarCardItem::resettaMazzo
 * resetta il gioco senza mischiare
 */
void CircolarCardItem::resettaMazzo() {
    cardNumber = this->mazzo.size() - 1;
    carteScoperte.clear();
    carteCoperte.clear();
    carteCoperte.append(mazzo);
    qint32 id=rand_generator.generate();
    QList<Card*> tmp;
    emit changeData(id, 2, tmp); // reset mazzo va reinizializzata anche la parte di gestione dell'undo
}

/**
 * Nel CircolarContainer non e' possibile ritornare le carte
 * quindi la isValid torna sempre falso
 * @param card
 * @return
 */
bool CircolarCardItem::isValid(Card *card) {
    return false;
}

/**
 * @brief CircolarCardItem::mischia
 * mischia il mazzo e resetta il gioco con la resettaMazzo()
 */
void CircolarCardItem::mischia() {
    QRandomGenerator generator(QDateTime::currentDateTime().toMSecsSinceEpoch());
    std::shuffle(mazzo.begin(), mazzo.end(), generator);
    resettaMazzo();
}