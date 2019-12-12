#ifndef FLOWER_H
#define FLOWER_H
#include <QGraphicsItem>
#include "iobjects.h"
#include <QObject>


class Flower : public QObject, public IObjects
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

signals:
    void RepaintObject(QGraphicsItem* item);
public:
    Flower(float x, float y);
    virtual ~Flower() override = default ;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    float _x = 0.;
    float _y = 0.;
    unsigned _flowerSize = 5;
    unsigned _lifeLevel = 100;
    bool _bloomed = false;


    // IObjects interface
public:
    void Work();

    // IObjects interface
public:
    void SetCoordinates(float x, float _y) override;
    float GetX() override;
    float GetY() override;
};


#endif // FLOWER_H

