//
// Created by Domenico Neri on 07/11/21.
//

#include "GameCards.h"

GameCards::GameCards(QWidget *parent) {
    //create the scene
    scene = new QGraphicsScene();
    setScene(scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // a white background
    scene->setBackgroundBrush(Qt::white);

    show();
}