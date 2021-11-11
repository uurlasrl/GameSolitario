//
// Created by Domenico Neri on 07/11/21.
//

#ifndef GAMESOLITARIO_GAME_H
#define GAMESOLITARIO_GAME_H

#include <QGraphicsView>
#include "CircolarCardItem.h"

class GameCards: public QGraphicsView{
    Q_OBJECT
public:
    GameCards();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
//    QGraphicsScene *scene;
    CircolarCardItem *ccard;
};


#endif //GAMESOLITARIO_GAME_H
