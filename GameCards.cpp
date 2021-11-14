//
// Created by Domenico Neri on 07/11/21.
//
#include <QGraphicsTextItem>
#include <QResizeEvent>
#include "GameCards.h"
#include "SemeItem.h"


GameCards::GameCards() {
    //create the scene
    QGraphicsScene *scene = new QGraphicsScene();
    setScene(scene);

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // a white background
    scene->setBackgroundBrush(Qt::white);

    scene->setSceneRect(0,0,800,600);
    qDebug()<<scene->width();
//    int posx = (this->rect().width() / 7) + this->rect().width() / 2;
//    int posy = 10;
    QColor *color=new QColor(QRandomGenerator::global()->bounded(256),
                             QRandomGenerator::global()->bounded(256),
                             QRandomGenerator::global()->bounded(256));

    ccard = new CircolarCardItem(scene->width()/7, this,
            color);
    int scW=scene->width()/7;
    for(int i=0;i<7;i++){
        boardItemList.append(
                new BoardItem(scW,color, ccard->distributeCards( i+1),this)
                );
    }

    for(int i=0;i<4;i++){
        semiOnBoard.append(new SemeItem(scW,CARD_HIGH,i,color,this));
    }

    show();
}

void GameCards::resizeEvent(QResizeEvent *event) {
    if(event->oldSize().width()!=-1){
        ccard->setBoardSize(event->size());
        for(int i=0;i<boardItemList.size();i++){
            boardItemList[i]->setBoardSize(event->size());
        }
        for(int i=0;i<4;i++){
            semiOnBoard[i]->setBoardSize(event->size());
        }
    }
    QGraphicsView::resizeEvent(event);
}

void GameCards::mousePressEvent(QMouseEvent *event) {

    QGraphicsItem *itemget = this->scene()->itemAt(event->position(), QTransform());
    if(itemget!= nullptr){
        CardStackItem *item=static_cast<CardStackItem*>(itemget);
        dragStarted = item->isCardDragableAt(event->position());
        if(dragStarted){
            dragingReleaseStarted = false;
            dragingCardList = item->getDragingCard(event->position());
            dragingItemFrom = item;
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void GameCards::mouseMoveEvent(QMouseEvent *event) {

    if(dragStarted && !dragingReleaseStarted){
        //TODO spostamento delle carte
        //event->position()
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GameCards::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsItem *itemget = this->scene()->itemAt(event->position(), QTransform());
    if(itemget== nullptr || dragingCardList.isEmpty()){
        dragStarted=false;
        dragingCardList.clear();
        dragingItemFrom= nullptr;
        return;
    }else{
    if(itemget==dragingItemFrom){ //e' un semplice click sull'item
        for(int i=0;i<4;i++){
            if(semiOnBoard[i]->isValid(dragingCardList.first())){
                semiOnBoard[i]->transferFrom(dragingItemFrom,dragingCardList.first());
                dragStarted=false;
                dragingCardList.clear();
                dragingItemFrom= nullptr;
                return;
            }
        }
    }
    if(dragStarted && !dragingReleaseStarted){
        dragingReleaseStarted = true;
            CardStackItem *item=static_cast<CardStackItem*>(itemget);
            if(item->isValid(dragingCardList[0])){
                item->transferFrom(dragingItemFrom,dragingCardList[0]);
            }

    }
    }
    QGraphicsView::mouseReleaseEvent(event);
}
