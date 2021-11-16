//
// Created by Domenico Neri on 07/11/21.
//

#include "CircolarCardItem.h"
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

/**
 * @brief CircolarCardItem::CircolarCardItem
 * inizializza il mazzo di carte
 */

CircolarCardItem::CircolarCardItem(float wi,QGraphicsView *parentview,QColor *c): CardStackItem(c), myw(wi), myh(100){
    mazzo.reserve(sizeof(Card *) * 52);
    for (int i = 0; i < 52; i++) {
        mazzo.append(new Card(i));
    }
    mischia();
    resettaMazzo();

    parentview->scene()->addItem(this);
    this->setPos(myw*5,0);
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
    emit changeData(id,3,CardList({card}),boundingRect()); //scopre carta

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
            CardList temp(carteCoperte);
            carteScoperte.push(carteCoperte.pop());
            emit changeData(id,4,temp,this->boundingRect()); //carte girate
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
    CardList tmp;
    scopriCard();
    emit changeData(id, 2, tmp,boundingRect()); // reset mazzo va reinizializzata anche la parte di gestione dell'undo
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

QRectF CircolarCardItem::boundingRect() const {
    //float myw= this->scene()->width()/7;
    return QRectF(0,0,myw*2,myh);
}

void CircolarCardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    if(!carteCoperte.isEmpty()){
        QRectF rect(boundingRect().x()+myw,boundingRect().y(),myw,myh);
        Card::paintBackCard(QRectF(myw,0,myw,myh),painter, option, widget,Card::resizedBackImage(myw-6-10, myh-10-10) );
        //painter->setBrush(Qt::darkRed);
//        QPen pen;
//        pen.setStyle( Qt::SolidLine );
//        pen.setWidth( 2 );
//        pen.setColor( Qt::black );
//        painter->setPen( pen);
//        painter->drawRoundedRect(boundingRect().x()+myw+3,boundingRect().y()+5,myw-6,boundingRect().height()-10,10.0,10.0);
    }
    if(!carteScoperte.isEmpty()){
        //se ci sono carte scoperte
        QRectF *cardPosition=new QRectF(0,0,myw,boundingRect().height());
        carteScoperte.last()->paint(QRectF(0,0,myw,myh),painter, option, widget);//,cardPosition
    }
}
void CircolarCardItem::setBoardSize(QSize newSize) {
    myw=newSize.width()/7;
    this->setPos(myw*5,0);
    this->scene()->setSceneRect(0,0,newSize.width(),newSize.height());
//    qDebug()<<"size:"<<newSize.width();
//    qDebug()<<"Scene size:"<<this->scene()->width();
}

bool CircolarCardItem::isCardDragableAt(QPointF point) {
    QPointF p = point - this->pos();
    return p.x()<myw;
    //return CardStackItem::isCardDragableAt(point);
}

CardList CircolarCardItem::getDragingCard(QPointF point) {
    QPointF p = point - this->pos();
    if(p.x()>=myw)
        return CardList();
    else
        return CardList({carteScoperte[0]});
    //return CardStackItem::getDragingCard(point);
}

CardList CircolarCardItem::distributeCards(int number) {
    CardList tmp;
    if(carteScoperte.size()==1&&carteCoperte.size()>=number){
        for( int i=0;i<number;i++){
            tmp.append(carteCoperte.pop());
        }
    }
    return tmp;
}
