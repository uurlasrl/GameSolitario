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

class CircolarCardItem;

#define CardList QList<Card*>

/* tipo della singola carta
 */
class Card{
    friend class CircolarCardItem;
public:
    enum CardColor: unsigned short {
        Cuori = 0,
        Quadri = 1,
        Fiori = 2,
        Picche = 3
    };

    CardColor getCardColor();
    int getCardNumber();
    void paint(QRectF *position, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
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
    explicit CardStackItem(QColor *color);
    //controllo se sono finite le carte
    bool isEmpty();
    //carte ancora coperte
    int sizeCoperte();
    // numero carte coperte
    int sizeScoperte();

    //trasferisce da un CardStackItem ad un altro se ci sono carte disponibili sull'altro container
    // e se la prima carta disponibile e' valida per il container corrente
    virtual bool transferFrom(CardStackItem *otherCardStack, Card *from);

    // verifica se una carta e' valida per il trasferimento
    virtual bool isValid(Card *) = 0;

    virtual bool isCardDragableAt(QPointF point) = 0;

    virtual CardList getDragingCard(QPointF point) = 0;

    CardList getCarteScoperte() const;


    signals:
        //viene emesso quando il container si modifica
        //aggiunta una carta, la carta viene tolta oppure per una qualunque
        //operazione che modifica l'aspetto grafico
        //si occupera' anche di fare la rollback di una mossa(forse)
        void changeData(qint32 eventID, int eventType, CardList data);

    protected:

    // contiene le carte coperte
    QStack<Card*> carteCoperte;
    QStack<Card*> carteScoperte;
    QRandomGenerator rand_generator;
    QColor *color;

private:

};



#endif
