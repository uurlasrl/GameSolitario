//
// Created by Domenico Neri on 07/11/21.
//

#include "BoardItem.h"
#include <QGraphicsView>
#include <QPicture>
#include <QImage>
#include <QUrl>

#define CART_STEP 5
BoardItem::~BoardItem() {}
BoardItem::BoardItem(float wi,QColor *color, CardList cards, QGraphicsView *parentview) : CardStackItem(color),  myw(wi), myh(CARD_HIGH){


    itemCapacity = cards.size();
    for(int i=0;i<itemCapacity;i++){
        if(i==0)
            carteScoperte.push(cards[0]);
        else
            carteCoperte.push(cards[i]);
    }
    parentview->scene()->addItem(this);
    this->setPos(myw*(itemCapacity-1),myh+(myh/CART_STEP));
}
BoardItem::BoardItem(float wi,QColor *color, CardGenerator *cardGenerator,int ic, QGraphicsView *parentview, QDataStream &dataStream) : CardStackItem(color),  myw(wi), myh(CARD_HIGH){

    itemCapacity = ic;
    CardStackItem::deserializeFrom(dataStream,cardGenerator);
    parentview->scene()->addItem(this);
    this->setPos(myw*(itemCapacity-1),myh+(myh/CART_STEP));
}

bool BoardItem::isValid(Card *card) {
    if(carteScoperte.isEmpty())
        return carteCoperte.isEmpty()&&card->getCardNumber()==13;
    else
        return (carteScoperte.last()->getCardNumber()-1==card->getCardNumber() && carteScoperte.last()->getCardColored()!=card->getCardColored());
}

void BoardItem::setBoardSize(QSize s) {

    //qDebug()<<s;
    myw = s.width()/7;
    myh = CARD_HIGH;
    this->setPos(myw*(itemCapacity-1),myh+(myh/CART_STEP));
}

QRectF BoardItem::boundingRect() const {
    qreal theX = 0;//(itemCapacity-1)*myw;
    qreal theY = 0;//myh+(myh/5);
    qreal wid= myw;
    int sizeCombined = carteScoperte.size()+carteCoperte.size()-1;
    qreal hei = myh+(sizeCombined*(myh/CART_STEP));

    return QRectF(theX,theY, wid, hei);
}

void BoardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    QRectF rect = boundingRect();
    if(!carteCoperte.isEmpty()){
        QImage backImage=Card::resizedBackImage(myw-6-10, myh-10-10);
        QImage image2=Card::resizedVoltiImages(myw-6-10, myh-10-10);
        for(int i=0;i<carteCoperte.size();i++){
            Card::paintBackCard(QRectF(0,(myh/CART_STEP)*i,myw,myh),painter, option, widget,backImage, image2);

        }
    }
    if(!carteScoperte.isEmpty()){
        for(int i=0;i<carteScoperte.size();i++){
            carteScoperte[i]->paint(QRectF(0.0,(carteCoperte.size()+i)*myh/CART_STEP,myw,myh),painter,option,widget);
        }
    }
}

bool BoardItem::isCardDragableAt(QPointF point) {
    return (point.y()-myh-(myh/CART_STEP))>((myh/CART_STEP)*carteCoperte.size());
}

QList<Card *> BoardItem::getDragingCard(QPointF point) {
    int number = (point.y()-myh-myh/CART_STEP)/(myh/CART_STEP);
    number -= carteCoperte.size();
    if(number>carteScoperte.size()-1){
        return CardList(1,carteScoperte.last());
    }
    CardList tmp;
    for(int i=number;i<carteScoperte.size();i++){
        tmp.append(carteScoperte[i]);
    }
    return tmp;
}

void BoardItem::scopriCartaIfEmpty(qint32 eventID) {
    if(carteScoperte.isEmpty() && !carteCoperte.isEmpty()){
        carteScoperte.push(carteCoperte.pop());
        qint32 id=eventID==0?rand_generator.bounded((qint32)1,(qint32)32000):eventID;
        emit changeData(id,Card::CardEventType::scopreCarta,CardList({carteScoperte[0]}),boundingRect());
    }
}
//void BoardItem::serializeTo(QDataStream &dataStream) {
//    //dataStream << myw;
//    //dataStream << myh;
//    //dataStream << itemCapacity;
//    CardStackItem::serializeTo(dataStream);
//}
