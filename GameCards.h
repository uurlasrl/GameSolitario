//
// Created by Domenico Neri on 07/11/21.
//

#ifndef GAMESOLITARIO_GAME_H
#define GAMESOLITARIO_GAME_H

#include <QGraphicsView>
#include "CircolarCardItem.h"
#include "BoardItem.h"
#include "SemeItem.h"

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
    QList<BoardItem*> boardItemList;
    QList<SemeItem*> semiOnBoard;

    QColor *m_color;
    CardList dragingCardList;
    bool dragStarted=false;
    bool dragingReleaseStarted=false;
    CardStackItem  *dragingItemFrom;

    void removeObjectsFromBoard();
    void addObjectsToBoard();
    void refreshMazzo();
    bool stoppedEvent();
    void stopEvent();
    void resumeEvent();
    bool m_disabedEvent;
private slots:
    void changedItem(qint32 eventID, int eventType, CardList data, QRectF);
    void restartGame();
};


#endif //GAMESOLITARIO_GAME_H
