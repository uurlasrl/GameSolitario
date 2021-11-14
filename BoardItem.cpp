//
// Created by Domenico Neri on 07/11/21.
//

#include "BoardItem.h"
#include <QGraphicsView>
#include <QPicture>
#include <QImage>
#include <QUrl>



BoardItem::BoardItem(float wi,QColor *color, CardList cards, QGraphicsView *parentview) : CardStackItem(color),  myw(wi), myh(CARD_HIGH){


    itemCapacity = cards.size();
    for(int i=0;i<itemCapacity;i++){
        if(i==0)
            carteScoperte.push(cards[0]);
        else
            carteCoperte.push(cards[i]);
    }
    parentview->scene()->addItem(this);
    this->setPos(myw*(itemCapacity-1),myh+(myh/6));
}

bool BoardItem::isValid(Card *card) {
    return
        (carteScoperte.isEmpty() && carteCoperte.isEmpty() && card->getCardNumber()==13) ||
        (!carteScoperte.isEmpty() && carteScoperte.last()->getCardNumber()+1==card->getCardNumber());
}

void BoardItem::setBoardSize(QSize s) {
    //qDebug()<<s;
    this->setPos(myw*(itemCapacity-1),myh+(myh/6));
    myw = s.width()/7;
    myh = CARD_HIGH;
}

QRectF BoardItem::boundingRect() const {
    qreal theX = 0;//(itemCapacity-1)*myw;
    qreal theY = 0;//myh+(myh/5);
    qreal wid= myw;
    int sizeCombined = carteScoperte.size()+carteCoperte.size();
    qreal hei = myh+(sizeCombined*(myh/6));

    return QRectF(theX,theY, wid, hei);
}

void BoardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {



    //myw-6-10,myh-10-10);
//    //disegna lo sfondo
//    painter->setBrush(Qt::lightGray);
//    painter->drawRoundedRect(boundingRect(),10.0,10.0);
//    qDebug()<<itemCapacity;
//    QPen pen;
//    pen.setStyle( Qt::SolidLine );
//    pen.setWidth( 2 );
//    pen.setColor( Qt::black );

    QRectF rect = boundingRect();
    if(!carteCoperte.isEmpty()){
        QImage backImage=Card::resizedBackImage(myw-6-10, myh-10-10);
        for(int i=0;i<carteCoperte.size();i++){
            Card::paintBackCard(QRectF(0,(myh/6)*i,myw,myh),painter, option, widget,backImage );
//            painter->drawImage(boundingRect().x()+3+5,boundingRect().y()+(i*myh/4)+5+5,backImage);
//            painter->setPen(pen);
//            painter->drawRoundedRect(boundingRect().x()+3,boundingRect().y()+(i*myh/4)+5,myw-6,myh-10,10.0,10.0);

        }
    }
    if(!carteScoperte.isEmpty()){
        for(int i=0;i<carteScoperte.size();i++){
            carteScoperte[i]->paint(QRectF(0.0,(carteCoperte.size()+i)*myh/6,myw,myh),painter,option,widget);
        }
    }
//    painter->setPen( pen);
//    if(!carteScoperte.isEmpty()){
//        //se ci sono carte scoperte
//        for(int i=0;i<carteScoperte.size();i++){
//            QRectF posi = boundingRect();
//            QRectF *cardPosition=new QRectF(
//                    posi.x(),
//                    posi.y()+((myh/4)*(i*carteCoperte.size())),
//                    myw,
//                    boundingRect().height());
//            carteScoperte[i]->paint(cardPosition,painter, option, widget);
//        }
//    }
}

bool BoardItem::isCardDragableAt(QPointF point) {
    return point.y()>((myh/6)*carteCoperte.size());
}

QList<Card *> BoardItem::getDragingCard(QPointF point) {
    int number = (point.y()-myh-myh/5)/(myh/6);
    number -= carteCoperte.size();
    CardList tmp;
    for(int i=number;i<carteScoperte.size();i++){
        tmp.append(carteScoperte[i]);
    }
    return tmp;
}