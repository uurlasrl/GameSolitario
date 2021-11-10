//
// Created by Domenico Neri on 07/11/21.
//

#ifndef GAMESOLITARIO_GAME_H
#define GAMESOLITARIO_GAME_H

#include <QGraphicsView>

class GameCards: public QGraphicsView{
    Q_OBJECT
public:
    GameCards(QWidget * parent = nullptr);

private:
    QGraphicsScene *scene;

};


#endif //GAMESOLITARIO_GAME_H
