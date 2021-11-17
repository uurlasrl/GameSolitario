//
// Created by Domenico Neri on 07/11/21.
//
#include <QGraphicsTextItem>
#include <QResizeEvent>
#include "GameCards.h"
#include "SemeItem.h"
#include <QPushButton>
#include <QVBoxLayout>


GameCards::GameCards() {
    stopEvent();
    //create the scene
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // a white background
    scene->setBackgroundBrush(Qt::white);

    scene->setSceneRect(0, 0, 800, 600);

    m_color = new QColor(QRandomGenerator::global()->bounded(256),
                         QRandomGenerator::global()->bounded(256),
                         QRandomGenerator::global()->bounded(256));

    ccard = new CircolarCardItem(scene->width() / 7, this,
                                 m_color);

    connect(ccard, &CircolarCardItem::changeData, this, &GameCards::changedItem);
    ccard->setObjectName("mazzo");

    addObjectsToBoard();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    QPushButton *pushButton = new QPushButton(this);
    pushButton->setText("reset");
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    layout->addWidget(pushButton);
    //pushButton->setGeometry(QRect(280, 140, 115, 32));
    show();

    QObject::connect(pushButton, &QPushButton::clicked, this, &GameCards::restartGame);
    resumeEvent();
}

void GameCards::refreshMazzo() {
    ccard->mischia();
}

void GameCards::removeObjectsFromBoard() {

    for (int i = 0; i < boardItemList.size(); i++) {
        BoardItem *bi = boardItemList[i];
        bi->disconnect();
        bi->setVisible(false);
        this->scene()->removeItem(bi);
        bi->deleteLater();
    }
    boardItemList.clear();
    for (int i = 0; i < semiOnBoard.size(); i++) {
        SemeItem *sm = semiOnBoard[i];
        sm->disconnect();
        sm->setVisible(false);
        scene()->removeItem(sm);
        sm->deleteLater();
    }
    semiOnBoard.clear();

}

void GameCards::addObjectsToBoard() {
    int scW = scene()->width() / 7;
    for (int i = 0; i < 7; i++) {
        BoardItem *bi = new BoardItem(scW, m_color, ccard->distributeCards(i + 1), this);
        bi->setObjectName(QString("BoardItem :" + QString::number(i)));
        boardItemList.append(bi);
        connect(bi, &BoardItem::changeData, this, &GameCards::changedItem);
    }

    for (int i = 0; i < 4; i++) {
        SemeItem *si = new SemeItem(scW, CARD_HIGH, i, m_color, this);
        si->setObjectName("SemeItem :" + QString::number(i));
        semiOnBoard.append(si);
        connect(si, &SemeItem::changeData, this, &GameCards::changedItem);
    }
}

void GameCards::restartGame() {
    stopEvent();
    removeObjectsFromBoard();
    refreshMazzo();
    addObjectsToBoard();
    QRectF rect = ccard->boundingRect();
    rect.moveTo(ccard->pos());
    qDebug() << rect;
    this->scene()->invalidate(rect);
    resumeEvent();
}

void GameCards::resizeEvent(QResizeEvent *event) {
    if (event->oldSize().width() != -1) {
        ccard->setBoardSize(event->size());
        for (int i = 0; i < boardItemList.size(); i++) {
            boardItemList[i]->setBoardSize(event->size());
        }
        for (int i = 0; i < 4; i++) {
            semiOnBoard[i]->setBoardSize(event->size());
        }
    }
    QGraphicsView::resizeEvent(event);
}

void GameCards::mousePressEvent(QMouseEvent *event) {
    if (stoppedEvent()) return;
    QGraphicsItem *itemget = this->scene()->itemAt(event->position(), QTransform());
    if (itemget != nullptr) {


        CardStackItem *item = static_cast<CardStackItem *>(itemget);
        qDebug() << " preso itam:" << item->objectName();
        dragStarted = item->isCardDragableAt(event->position());
        if (dragStarted) {
            dragingReleaseStarted = false;
            dragingCardList = item->getDragingCard(event->position());
            qDebug() << "preso:";
            for (int i = 0; i < dragingCardList.size(); i++) {
                qDebug() << dragingCardList[i]->getCardNumber() << " " << dragingCardList[i]->getColorName() << ",";
            }
            dragingItemFrom = item;
        } else {
            qDebug() << "non sono state prese carte";
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void GameCards::mouseMoveEvent(QMouseEvent *event) {
    if (stoppedEvent()) return;

    if (dragStarted && !dragingReleaseStarted) {
        //TODO spostamento delle carte
        //event->position()
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GameCards::mouseReleaseEvent(QMouseEvent *event) {
    if (stoppedEvent()) return;

    QGraphicsItem *itemget = this->scene()->itemAt(event->position(), QTransform());

    if (dragingCardList.isEmpty()) {
        if (itemget == nullptr) {
            dragStarted = false;
            dragingCardList.clear();
            dragingItemFrom = nullptr;
            return;
        } else if (itemget == this->ccard) {
            ccard->scopriCard();

            dragStarted = false;
            dragingCardList.clear();
            dragingItemFrom = nullptr;
            return;
        }
    } else {
        if (itemget == dragingItemFrom) { //e' un semplice click sull'item
            if (!semiOnBoard.contains(itemget)) //non lo faccio se si tratta di un mazzetto dei semi
                for (int i = 0; i < 4; i++) {
                    if (semiOnBoard[i]->isValid(dragingCardList.first())) {
                        semiOnBoard[i]->transferFrom(dragingItemFrom, dragingCardList.first());
                        dragStarted = false;
                        dragingCardList.clear();
                        dragingItemFrom = nullptr;
                        return;
                    }
                }
            for (int i = 0; i < boardItemList.size(); i++) {
                if (boardItemList[i] != itemget && boardItemList[i]->isValid(dragingCardList.first())) {
                    boardItemList[i]->transferFrom(dragingItemFrom, dragingCardList.first());
                    dragStarted = false;
                    dragingCardList.clear();
                    dragingItemFrom = nullptr;
                    return;
                }
            }
        }
        if (dragStarted && !dragingReleaseStarted) {
            dragingReleaseStarted = true;
            CardStackItem *item = static_cast<CardStackItem *>(itemget);
            if (item->isValid(dragingCardList[0])) {
                item->transferFrom(dragingItemFrom, dragingCardList[0]);
            }
            dragStarted = false;
            dragingCardList.clear();
            dragingItemFrom = nullptr;
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GameCards::changedItem(qint32 eventID, int eventType, QList<Card *> data, QRectF area) {
    //this->invalidateScene(area);
    this->scene()->invalidate(area);
    CardStackItem *send = dynamic_cast<CardStackItem *>(sender());
    qDebug() << "invalidate da " << send->objectName() << " area:" << area;
}

bool GameCards::stoppedEvent() {
    return m_disabedEvent;
}

void GameCards::stopEvent() {
    m_disabedEvent = true;
}

void GameCards::resumeEvent() {
    m_disabedEvent = false;
}