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
    void GenerateClone(Flower* flower);
    void DeleteFlower(Flower* flower);
public:
    Flower(float x, float y, World* worldPtr);
    virtual ~Flower() override = default ;
    float GiveNectar();
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    float _x = 0.f;
    float _y = 0.f;
    float _health;
    unsigned _flowerSize = 5;
//    bool _bloomed = false;
    float _scaleX, _scaleY;
    float _maxCapacityOfNectar = 20.0;
    float _containsNectar = _maxCapacityOfNectar;
    float _drawingX;
    float _drawingy;
    float _decreasesHealth = 0.07f;
    float _lifeLevel = 20.f;
    int _leftToBirth = 3;
    unsigned _stepsFromLastClone = 0;

    const float _COLLECT_SPEED = 0.7f;
    const unsigned _STEPS_TO_NEXT_POSIBLE_CLONING = 40;
    const float _MIN_START_HEALTH = 10.f;
    const float _MAX_START_HEALTH = 25.f;

    bool _firstDraw = true;
    bool _posibleToClone = true;

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

