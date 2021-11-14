//
// Created by Domenico Neri on 07/11/21.
//

#include "SemeItem.h"
#include <QGraphicsView>

SemeItem::SemeItem(float wi, float hi, int id, QColor *color,QGraphicsView *parentview): CardStackItem(color),myw(wi), myh(hi), m_id(id) {
    parentview->scene()->addItem(this);
    this->setPos(myw*id,0);
}

bool SemeItem::isValid(Card *card) {
    if(carteScoperte.isEmpty())
        return card->getCardNumber() == 1;
    return carteScoperte.last()->getCardNumber()+1 == card->getCardNumber() && carteScoperte[0]->getCardColor() == card->getCardColor();
}

QRectF SemeItem::boundingRect() const {
    return QRectF(0.0,0.0,myw,myh);
}

void SemeItem::setBoardSize(QSize s) {
    myw = s.width()/7;
    myh = CARD_HIGH;
    this->setPos(myw*m_id,0);
}

void SemeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();
    if(carteScoperte.isEmpty()){
        QPen pen;
        pen.setStyle( Qt::SolidLine );
        pen.setWidth( 2 );
        pen.setColor( Qt::black );
        painter->setPen(pen);
        painter->drawRoundedRect(   3,     5, rect.width()-6,  rect.height()-10 ,10.0,10.0);
        }else{
        carteScoperte.last()->paint(rect,painter,option,widget);
    }
}

bool SemeItem::isCardDragableAt(QPointF point) {
    return !carteScoperte.isEmpty();
}

CardList SemeItem::getDragingCard(QPointF point) {
    if(!carteScoperte.isEmpty()){
        return CardList({carteScoperte.last()});
    }
    return CardList();
}

//bool SemeItem::transferFrom(CardStackItem *otherCardStack, Card *from) {
//    if(otherCardStack->sizeScoperte()>0&&otherCardStack->getCarteScoperte().last()==from)
//        return CardStackItem::transferFrom(otherCardStack, from);
//    return true;
//}