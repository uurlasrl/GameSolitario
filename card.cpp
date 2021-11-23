#include "card.h"
#include <algorithm>


QImage *Card::backImage = nullptr;
QList<QImage *> Card::voltiImages;

/**
 * @brief Card::Card
 * inizializza la carta
 * @param iid
 */
Card::CardColor Card::getCardColor() {
    return cardColor;
}

int Card::getCardId() {
    return id;
}

int Card::getCardNumber() {
    return value + 1;
}

QImage Card::resizedBackImage(int x, int y) {
    return Card::backImage->scaled(x, y);
}

QImage Card::resizedVoltiImages(int x, int y) {
    return Card::voltiImages[QRandomGenerator::global()->bounded(7)]->scaled(x - 20, y - 30);
}

Card::Card(int iid) {
    //QPixmap pixmap
    //QImage  image(myw-6-10, myh-10-10,QImage::Format_RGB32);
    if (Card::backImage == nullptr) {
        Card::backImage = new QImage(":/images/card.png");
        Card::voltiImages.append(new QImage(":/images/chiara.jpeg"));
        Card::voltiImages.append(new QImage(":/images/domenico.jpeg"));
        Card::voltiImages.append(new QImage(":/images/davide.jpeg"));
        Card::voltiImages.append(new QImage(":/images/enrico.jpeg"));
        Card::voltiImages.append(new QImage(":/images/franco.jpeg"));
        Card::voltiImages.append(new QImage(":/images/nadine.jpeg"));
        Card::voltiImages.append(new QImage(":/images/sandro.jpeg"));
        Card::voltiImages.append(new QImage(":/images/sara.jpeg"));
    }

    id = iid;
    value = iid % 13;
    cardColor = static_cast<CardColor>(iid / 13);

}

void
Card::paintBackCard(QRectF boundingRect, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget,
                    const QImage &img, const QImage &img1) {
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawRoundedRect(boundingRect.x() + 3, boundingRect.y() + 5, boundingRect.width() - 6,
                             boundingRect.height() - 10, 10.0, 10.0);
    painter->setBrush(Qt::white);
    painter->drawImage(boundingRect.x() + 3 + 5, boundingRect.y() + 5 + 5, img);
    painter->drawImage(boundingRect.x() + 3 + 15, boundingRect.y() + 5 + 35, img1);

}

void Card::paint(QRectF boundingRect, QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {//QRectF *position,


    painter->setBrush(Qt::white);
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawRoundedRect(boundingRect.x() + 3, boundingRect.y() + 5, boundingRect.width() - 6,
                             boundingRect.height() - 10, 10.0, 10.0);
//    painter->setBrush(Qt::white);
//    painter->drawImage(         3+5,   5+5, img);


    if (this->getCardNumber() > 10) {
        paintFigura(&boundingRect, painter, option, widget);
    } else {
        paintCarta(&boundingRect, painter, option, widget);
    }

}

void Card::paintFigura(QRectF *position, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QImage imgIcon;
    QImage figura;
    QString lettera;
    QString simbolo = getColorName();
    switch (value) {
        case 10:
            lettera = "J";
            break;
        case 11:
            lettera = "Q";
            break;
        case 12:
            lettera = "K";
    }

    QFont f;
    f.setBold(true);
    painter->setFont(f);

    painter->drawText(position->x() + 15, position->y() + 24, lettera);
    imgIcon.load(":/images/" + simbolo.toLower() + ".png");
    imgIcon = imgIcon.scaled(15, 15);
    figura.load(":/images/" + lettera + simbolo + ".png");
    painter->drawImage(position->x() + 30, position->y() + 15, imgIcon);
}

QString Card::getColorName() {
    switch (this->getCardColor()) {
        case CardColor::Quadri:
            return QString("Quadri");
        case CardColor::Fiori:
            return QString("Fiori");
        case CardColor::Picche:
            return QString("Picche");
        case CardColor::Cuori:
            return QString("Cuori");
    }
    return QString();
}

void Card::paintCarta(QRectF *position, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QImage imgIcon;
    QString simbolo = getColorName();
    QFont f;
    f.setBold(true);
    painter->setFont(f);

    painter->drawText(position->x() + 12, position->y() + 24, QString::number(this->getCardNumber()));
//    if(value<9)
//        painter->drawText(15,15,QString(strchr("123456789",this->value)));
//    else
//        painter->drawText(15,15,QString("10"));

    imgIcon.load(":/images/" + simbolo.toLower() + ".png");
    imgIcon = imgIcon.scaled(15, 15);
    painter->drawImage(position->x() + 30, position->y() + 15, imgIcon);

}

Card::Colored Card::getCardColored() {
    return (this->cardColor == Card::Fiori || this->cardColor == Card::Picche) ? Card::Nere : Card::Rosse;
}

CardStackItem::~CardStackItem() {}


CardStackItem::CardStackItem(QColor *c) : color(c) {
    rand_generator.seed(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

/**
 * controlla se lo stack e' vuoto
 * @return
 */
bool CardStackItem::isEmpty() {
    return carteScoperte.isEmpty() && carteCoperte.isEmpty();
}

int CardStackItem::sizeCoperte() {
    return carteCoperte.size();
}

int CardStackItem::sizeScoperte() {
    return carteCoperte.size();
}

/**
 * @brief CardStackItem::transferFrom
 * trasferisce le carta da un altro stack
 * @param otherCardStack
 * @return torna vero se il trasferimanto e' andato a buon fine altrimenti falso
 */
bool CardStackItem::transferFrom(CardStackItem *otherCardStack, Card *from, qint32 eventID) {
    QRectF rectOther(otherCardStack->boundingRect());
    rectOther.moveTo(otherCardStack->pos());
    QRectF rectThis(boundingRect());
    rectThis.moveTo(pos());
    //va a cercare l'indice della card
    Card *c;
    int idx = otherCardStack->carteScoperte.size() - 1;
    while (idx >= 0) {
        c = otherCardStack->carteScoperte[idx];
        if (c == from) {
            break;
        }
        idx--;
    }
    if (idx < 0)return false; //non trova la card
    if (!this->isValid(c)) return false; //l'elemento da inserire non e' valido

    // calcola il numero di card da togliere
    int len = idx = otherCardStack->carteScoperte.size() - idx;

    // riversa le card da togliere in una lista temporanea
    CardList temp;
    while (idx > 0) {
        temp.append(otherCardStack->carteScoperte.pop());
        idx--;
    }
    idx = len - 1;
    while (idx >= 0) {
        this->carteScoperte.push(temp[idx--]);
    }

    // comunica le modifiche per un visitors o per la modifica della visualizzazione
    qint32 id = eventID == 0 ? rand_generator.bounded((qint32) 1, (qint32) 32000) : eventID;


    //rimuovo la carta dall'altro item
    QRectF rectAfterOther(otherCardStack->boundingRect());
    rectAfterOther.moveTo(otherCardStack->pos());
    emit otherCardStack->changeData(id, Card::CardEventType::toglieCarta, temp,
                                    rectOther.united(rectAfterOther));//rimozione carte

    otherCardStack->scopriCartaIfEmpty(id);

    //aggiungo la carta in questo item
    QRectF rectAfterThis(boundingRect());
    rectAfterThis.moveTo(pos());
    emit changeData(id, Card::CardEventType::aggiungeCarte, temp, rectThis.united(rectAfterThis));//aggiunta carte



    return true;
}

CardList CardStackItem::getCarteScoperte() const {
    CardList tmp(carteScoperte);
    return tmp;
}

void CardStackItem::resetEvent(GameEvent *event) {
    switch (event->eventType) {
        case Card::CardEventType::aggiungeCarte: {
            //bisogna togliere le carte che sono state aggiunte dalle carte scoperte
            for (int i = 0; i < event->data.size(); i++)
                carteScoperte.pop();
            break;
        }
        case Card::CardEventType::toglieCarta: {
            //bisogna aggiungere le carte alle carte scoperte
            for (int i = 0; i < event->data.size(); i++) {
                carteScoperte.push(event->data[event->data.size() - i - 1]);
            }
            break;
        }
        case Card::CardEventType::scopreCarta: {
            //bisogna ricoprire la carta
            carteCoperte.push(carteScoperte.pop());

            break;
        }
        case Card::CardEventType::giraCarteDelMazzo: {
            //bisogna riposizionare il mazzo come prima
            while (!carteCoperte.isEmpty())
                carteScoperte.push(carteCoperte.pop());
            break;
        }
    }
}
void CardStackItem::deserializeFrom(QDataStream &dataStream, CardGenerator *cardGenerator){
    carteScoperte.clear();
    qsizetype nScoperte;
    dataStream >> nScoperte;
    for (int i = 0; i < nScoperte; i++) {
        unsigned short id;
        dataStream >> id;
        carteScoperte.append(cardGenerator->getCardById(id));
    }
    carteCoperte.clear();
    qsizetype nCoperte;
    dataStream >> nCoperte;
    for (int i = 0; i < nCoperte; i++) {
        unsigned short id;
        dataStream >> id;
        carteScoperte.append(cardGenerator->getCardById(id));
    }
}
void CardStackItem::serializeTo(QDataStream &dataStream) {
    //dataStream << color;
    dataStream << carteScoperte.size();
    for (int i = 0; i < carteScoperte.size(); i++) {
        dataStream << carteScoperte[i]->getCardId();
    }
    dataStream << carteCoperte.size();
    for (int i = 0; i < carteCoperte.size(); i++) {
        dataStream << carteCoperte[i]->getCardId();
    }
}

void GameEvent::serilizeTo(QDataStream &dataStream) {
    dataStream << eventID;
    dataStream << eventType;
    dataStream << area;
    dataStream << data.size();
    for (int i = 0; i < data.size(); i++) {
        dataStream << data[i]->getCardId();
    }

}
