//
// Created by Domenico Neri on 07/11/21.
//
#include <QGraphicsTextItem>
#include <QResizeEvent>
#include "GameCards.h"
#include "SemeItem.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>
#include <QGuiApplication>
#include <QStandardPaths>
#include <QFile>
#include <QDataStream>


GameCards::GameCards() {
    stopEvent();

    qDebug() << "Application started";

    //create the scene
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // a white background
    scene->setBackgroundBrush(Qt::white);

    scene->setSceneRect(0, 0, 800, 600);

    // se non si puo' aprire il file significa che non esiste
    m_color = new QColor(QRandomGenerator::global()->bounded(256),
                         QRandomGenerator::global()->bounded(256),
                         QRandomGenerator::global()->bounded(256));

    bool initialized = true;
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto fileName = path + "/backupFile.dat";
    QFile dataFile(fileName);
    if (dataFile.open(QIODevice::ReadOnly)) {
        //esiste un file di salvataggio
        //quindi effettuo il ripristino dell'applicazione
        QDataStream dataStream(&dataFile);
        if (dataStream.version() == QDataStream::Qt_6_1) {
            ccard = new CircolarCardItem(scene->width() / 7, this, m_color, dataStream);
            ccard->setObjectName("mazzo");
            QHash<QString, CardStackItem *> itemByName;
            itemByName["mazzo"] = ccard;

            addObjectsToBoard(dataStream, itemByName);


            qsizetype nEvent;
            dataStream >> nEvent;
            for (int i = 0; i < nEvent; i++) {
                qint32 eventId;
                dataStream >> eventId;
                eventIdStack.append(eventId);
            }
            dataStream >> nEvent;
            QList<GameEvent *> *gameEventList;
            for (int i = 0; i < nEvent; i++) {
                qsizetype nEventList;
                dataStream >> nEventList;
                gameEventList = new QList<GameEvent *>();
                for (int j = 0; i < nEventList; j++) {

                    qint32 eventId;
                    int ty;
                    CardList data;
                    qsizetype sizedt;
                    CardStackItem *send;

                    dataStream >> eventId;
                    dataStream >> ty;
                    dataStream >> sizedt;
                    for (int i = 0; i < sizedt; i++) {
                        unsigned short id;
                        dataStream >> id;
                        data.append(ccard->getCardById(id));
                    }
                    QString name;
                    dataStream >> name;
                    send = itemByName[name];

                    gameEventList->append(new GameEvent(eventId, ty, data, send->boundingRect(), send));
                }
                eventRepository[]
            }
            initialized = true;
        }
        dataFile.close();
    }

    if (!initialized) {

        ccard = new CircolarCardItem(scene->width() / 7, this, m_color);

        connect(ccard, &CircolarCardItem::changeData, this, &GameCards::changedItem);
        ccard->setObjectName("mazzo");

        addObjectsToBoard();
    }


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignRight | Qt::AlignBottom);


    QPushButton *resetButton = new QPushButton("reset", this);
    resetButton->setObjectName(QString::fromUtf8("pushButton"));
    QPushButton *rollbackButton = new QPushButton("rolback", this);
    QObject::connect(resetButton, &QPushButton::clicked, this, &GameCards::restartGame);
    QObject::connect(rollbackButton, &QPushButton::clicked, this, &GameCards::rollbackGame);

    resetButton->show();
    rollbackButton->show();


    rollbackButton->setObjectName(QString::fromUtf8("pushButton"));
    layout->addWidget(rollbackButton);
    layout->addWidget(resetButton);

    show();

    //QGuiApplication::applicationStateChanged()
    QObject::connect(qGuiApp, &QGuiApplication::applicationStateChanged, [this](Qt::ApplicationState state) {
        qDebug() << "** evento:" << state;
        auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        auto fileName = path + "/backupFile.dat";
        QFile dataFile(fileName);
        // qui va salvato lo stato dell'applicazione
        if (state == Qt::ApplicationState::ApplicationInactive) {
            if (dataFile.open(QIODevice::WriteOnly)) {
                QDataStream dataStream(&dataFile);
                dataStream.setVersion(QDataStream::Qt_6_1);
                ccard->serializeTo(dataStream);
                for (int i; i < boardItemList.size(); i++) {
                    boardItemList[i]->serializeTo(dataStream);
                }
                for (int i; i < semiOnBoard.size(); i++) {
                    semiOnBoard[i]->serializeTo(dataStream);
                }

                dataStream << eventIdStack.size();
                for (int i = 0; i < eventIdStack.size(); i++) {
                    qint32 eventID = eventIdStack[i];
                    dataStream << eventID;
                    for (int j = 0; i < eventRepository[eventID]->size(); j++) {
                        eventRepository[eventID]->value(j)->serializeTo(dataStream);
                    }
                }
                dataFile.close();
            }
        }
    });


    resumeEvent();
}

void GameCards::refreshMazzo(qint32 eventID) {
    ccard->mischia(eventID);
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

void GameCards::addObjectsToBoard(QDataStream &dataStream, QHash<CardStackItem *> &itemByName) {
    int scW = scene()->width() / 7;
    boardItemList.clear();
    for (int i = 0; i < 7; i++) {
        BoardItem *bi = new BoardItem(scW, m_color, ccard, i + 1, this, dataStream);
        bi->setObjectName(QString("BoardItem :" + QString::number(i)));
        itemByName["BoardItem :" + QString::number(i)] = bi;
        boardItemList.append(bi);
        connect(bi, &BoardItem::changeData, this, &GameCards::changedItem);
    }

    for (int i = 0; i < 4; i++) {
        SemeItem *si = new SemeItem(scW, CARD_HIGH, i, m_color, this, ccard, dataStream);
        si->setObjectName("SemeItem :" + QString::number(i));
        itemByName["SemeItem :" + QString::number(i)] = si;
        semiOnBoard.append(si);
        connect(si, &SemeItem::changeData, this, &GameCards::changedItem);
    }
}

void GameCards::restartGame() {
    stopEvent();
    clearHistoryEvents();
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
//reset dati evento

    dragStarted = false;
    dragingCardList.clear();
    dragingItemFrom = nullptr;

    QGraphicsItem *itemget = this->scene()->itemAt(event->position(), QTransform());
    if (itemget != nullptr) {
        CardStackItem *item = static_cast<CardStackItem *>(itemget);
//        qDebug() << " preso itam:" << item->objectName();
        dragStarted = item->isCardDragableAt(event->position());
        if (dragStarted) {
            dragingReleaseStarted = false;
            dragingCardList = item->getDragingCard(event->position());
//            qDebug() << "preso:";
//            for (int i = 0; i < dragingCardList.size(); i++) {
//                qDebug() << dragingCardList[i]->getCardNumber() << " " << dragingCardList[i]->getColorName() << ",";
//            }
            dragingItemFrom = item;
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
    while (true) {
        if (dragingCardList.isEmpty()) {
            if (itemget == nullptr) {
                break;
            } else if (itemget == this->ccard) {
                ccard->scopriCard();
                break;
            }
        } else {
            if (itemget == dragingItemFrom) { //e' un semplice click sull'item
                if (!semiOnBoard.contains(itemget)) //non lo faccio se si tratta di un mazzetto dei semi
                    for (int i = 0; i < 4; i++) {
                        if (semiOnBoard[i]->isValid(dragingCardList.first())) {
                            semiOnBoard[i]->transferFrom(dragingItemFrom, dragingCardList.first());
                            break;
                        }
                    }
                for (int i = 0; i < boardItemList.size(); i++) {
                    if (boardItemList[i] != itemget && boardItemList[i]->isValid(dragingCardList.first())) {
                        boardItemList[i]->transferFrom(dragingItemFrom, dragingCardList.first());
                        break;
                    }
                }
            }
            if (dragStarted && !dragingReleaseStarted && itemget != nullptr) {
                dragingReleaseStarted = true;
                CardStackItem *item = static_cast<CardStackItem *>(itemget);
                if (item->isValid(dragingCardList[0]) && item != nullptr) {
                    item->transferFrom(dragingItemFrom, dragingCardList[0]);
                    break;
                }
            }
        }
        break;
    }
    dragStarted = false;
    dragingCardList.clear();
    dragingItemFrom = nullptr;
    QGraphicsView::mouseReleaseEvent(event);
}

void GameCards::changedItem(qint32 eventID, int eventType, QList<Card *> data, QRectF area) {
    //this->invalidateScene(area);

    CardStackItem *send = dynamic_cast<CardStackItem *>(sender());
    if (send != nullptr && eventType != Card::CardEventType::resettaMazzo) {
// va memorizzato l'evento
        GameEvent *ge = new GameEvent(eventID, eventType, data, area, send);
        if (eventIdStack.contains(eventID)) {
            QList<GameEvent *> *listaAction = eventRepository[eventID];
            listaAction->append(ge);
        } else {
            //evento nuovo
            QList<GameEvent *> *tmp = new QList<GameEvent *>();
            tmp->append(ge);
            eventIdStack.append(eventID);
            eventRepository.insert(eventID, tmp);
        }
        qDebug() << "aggiunto l'evento " << ge->eventID << " da item " << (send->objectName()) << " tipo "
                 << ge->eventType;
    }
    //QTimer::singleShot(500,[this,area](){
    this->scene()->invalidate();
    //});
//    qDebug() << "invalidate da " << send->objectName() << " area:" << area;
}

void GameCards::clearHistoryEvents() {
    while (!eventIdStack.isEmpty()) {
        qint32 ev = eventIdStack.last();
        eventIdStack.removeLast();
        QList<GameEvent *> *tmp = eventRepository[ev];
        for (int i = 0; i < tmp->size(); i++) {
            delete tmp->value(i);
        }
        tmp->clear();
        delete tmp;
    }
    eventRepository.clear();
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

void GameCards::rollbackGame() {
    if (!eventIdStack.isEmpty()) {
        qint32 ev = eventIdStack.last();
        eventIdStack.removeLast();
        QList<GameEvent *> *tmp = eventRepository[ev];
        eventRepository.remove(ev);
        for (int i = 0; i < tmp->size(); i++) {
            GameEvent *ge = tmp->value(tmp->size() - i - 1);
            ge->sender->resetEvent(ge);
            this->scene()->invalidate(ge->area);
            delete ge;
        }
        tmp->clear();
        delete tmp;
    }
}

