#ifndef GAMESOLITARIO_CARD_H
#define GAMESOLITARIO_CARD_H

#include <QObject>
#include <QList>
#include <QStack>
#include <QRandomGenerator>
#include <QDateTime>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QColor>
#include <QImage>



class CircolarCardItem;
class Card;
class CardStackItem;

#define CardList QList<Card*>
#define CARD_HIGH 100

struct GameEvent{
    GameEvent(qint32 id,int ty,CardList dt, QRectF box,CardStackItem *send):
            eventID(id), eventType(ty), data(dt), area(box),sender(send){};
    qint32 eventID;
    int eventType;
    CardList data;
    QRectF area;
    CardStackItem *sender;
};

/* tipo della singola carta
 */
class Card{
    friend class CircolarCardItem;
public:
    enum CardEventType: qint32 {
        aggiungeCarte = 0,
        toglieCarta = 1,
        resettaMazzo = 2,
        scopreCarta = 3,
        giraCarteDelMazzo = 4

    };
    enum CardColor: unsigned short {
        Cuori = 0,
        Quadri = 1,
        Fiori = 2,
        Picche = 3
    };
    enum Colored: unsigned short {
        Rosse = 0,
        Nere = 1
    };

    static QImage *backImage;
    static QList<QImage*> voltiImages;
    static QImage resizedBackImage(int, int);
    static QImage resizedVoltiImages(int, int);

    CardColor getCardColor();
    Colored getCardColored();
    QString getColorName();
    int getCardNumber();
    int getCardId();
    void paint(QRectF boundingRect,QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//QRectF *position
    void paintFigura(QRectF *position, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintCarta(QRectF *position, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    static void paintBackCard(QRectF boundingRect, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const QImage &img, const QImage &img1);
    //static void paintBackOfTheCard(QRectF *position, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:
    Card(int iid);
    unsigned short id;
    unsigned short value;
    CardColor cardColor;

};


/**
 * @brief The CardStackItem class
 * Card stack deve essere la classe padre sia delle carte messe a terra sia delle carte
 * che sono posizionate in alto per raccogliere i 4 semi
 * Intanoto contengono uno stach di carte che eventualmente possono essere spostate tra un CardStackItem all'altro
 * e poi decidono con isEmpty che e' virtuale
 */
class CardStackItem: public QObject, public QGraphicsItem{
    Q_OBJECT
public:
    virtual ~CardStackItem();
    explicit CardStackItem(QColor *color);
    //controllo se sono finite le carte
    bool isEmpty();
    //carte ancora coperte
    int sizeCoperte();
    // numero carte coperte
    int sizeScoperte();

    //resetEventi
    void resetEvent(GameEvent *event);

    //trasferisce da un CardStackItem ad un altro se ci sono carte disponibili sull'altro container
    // e se la prima carta disponibile e' valida per il container corrente
    virtual bool transferFrom(CardStackItem *otherCardStack, Card *from, qint32 eventID=0);

    // verifica se una carta e' valida per il trasferimento
    virtual bool isValid(Card *) = 0;

    virtual bool isCardDragableAt(QPointF point)=0;

    virtual CardList getDragingCard(QPointF point)=0;
    virtual void scopriCartaIfEmpty(qint32 eventID=0)=0;

    virtual void serializeTo(QDataStream &dataSteream);

    CardList getCarteScoperte() const;


    signals:
        //viene emesso quando il container si modifica
        //aggiunta una carta, la carta viene tolta oppure per una qualunque
        //operazione che modifica l'aspetto grafico
        //si occupera' anche di fare la rollback di una mossa(forse)
        void changeData(qint32 eventID, int eventType, CardList data, QRectF);

    protected:

    // contiene le carte coperte
    QStack<Card*> carteCoperte;
    QStack<Card*> carteScoperte;
    QRandomGenerator rand_generator;
    QColor *color;

private:

};



#endif
