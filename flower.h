#ifndef FLOWER_H
#define FLOWER_H
#include <QGraphicsItem>
#include "iobjects.h"
#include <QObject>

class World;

class Flower : public QObject, public IObjects
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

signals:
    void RepaintObject(QGraphicsItem* item);
public:
    Flower(float x, float y, World* worldPtr);
    virtual ~Flower() override = default ;
    float GiveNectar();
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
    float _scaleX, _scaleY;
    float _maxCapacityOfNectar = 100.0;
    float _containsNectar = _maxCapacityOfNectar;
    float _drawingX;
    float _drawingy;
    const float _COLLECT_SPEED = 0.5f;
    bool _firstDraw = true;


    // IObjects interface
public:
    void Work();

    // IObjects interface
public:
    void SetCoordinates(float x, float y) override;
    float GetX() override;
    float GetY() override;
};


#endif // FLOWER_H

